/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

#include "Physics/SuppportShape/ObbSupportShape.h"
#include "RigidbodySystem.h"
#include "GameObjects/GameObject.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/Gjk/Gjk.h"
#include "CantDebug/CantDebug.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"
unsigned const RigidbodySystem::static_type = BaseSystem::numberOfTypes++;

#include "Components/TransformComponent.h"
#include "Components/RigidbodyComponent.h"



RigidbodySystem::RigidbodySystem() : 
	BaseSystem(), m_timeAccumulator(0.0f)
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<RigidbodyComponent>();
	Push_required_comp<MeshComponent>();

	EventManager::Get()->SubscribeEvent<KeyEvent>(this,
		std::bind(&RigidbodySystem::OnKeyDown, this, std::placeholders::_1));
}


void RigidbodySystem::Register_GameObject(GameObject *go)
{
	TransformComponent* transform = go->GetComponent<TransformComponent>();
	RigidbodyComponent* rigidbody = go->GetComponent<RigidbodyComponent>();
	MeshComponent* mesh = go->GetComponent<MeshComponent>();

	BaseSystemCompNode* component_node = new RigidbodyCompNode(transform, rigidbody, mesh);
	component_node->m_goID = go->GetId();

	// inertia tensors 
	const Vector3 scale = transform->GetScale();
	rigidbody->InitInertiaTensor(scale.x, scale.y, scale.z);

	// register with dynamic aabb
	SpatialPartitionData data1;
	data1.m_Aabb = rigidbody->m_aabb;
	data1.m_ClientData = rigidbody;
	m_broadPhase.InsertData(rigidbody->m_dynamicAabbTreeKey, data1);
	
	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void RigidbodySystem::RegisterAppRenderer(AppRenderer * renderer)
{
	m_pAppRenderer = renderer;
}


//This one is not called one per object in the system, but once per system.
//Access all the components data and stuff through the unordered_map
void RigidbodySystem::LateUpdate(float dt) 
{
#ifdef DEVELOPER
	if (m_isPaused && !m_isAdvanceOneStep)
	{
		return;
	}
#endif
	m_timeAccumulator += dt;
	while (m_timeAccumulator >= PhysicsUtils::Consts::fixedTimeStep)
	{
#ifdef DEVELOPER
		if (m_isPaused)
		{
			if (m_isAdvanceOneStep)
			{
				m_isAdvanceOneStep = false;
			}
			else
			{
				m_timeAccumulator -= dt;
				m_timeAccumulator += PhysicsUtils::Consts::fixedTimeStep;
				return;
			}
		}
#endif
#pragma region BroadPhaase
		// update all the aabb of all objects and dynamic aabb tree
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
#ifdef DEVELOPER
			if (rigidbodyNode == nullptr)
			{
				DEBUG_LOG("RigidbodySystem::LateUpdate(): rigidbodyNode == nullptr while updating aabb.");
			}
#endif			
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;
			TransformComponent* transform = rigidbodyNode->m_transform;
			
			MeshComponent* mesh = rigidbodyNode->m_mesh;
#ifdef DEVELOPER
			if (rigidbody == nullptr || transform == nullptr || mesh == nullptr)
			{
				DEBUG_LOG("RigidbodySystem::LateUpdate(): while updating aabb one or more of the following is a nullptr: rigidbody, mesh, transform.");
			}
#endif			
			
			if (!mesh->GetModel())
			{
				continue;
			}
			// updating aabb
			rigidbody->m_aabb = mesh->GetModel()->GetAABB(); // reset to object space
			Matrix model = transform->GetModel();
			rigidbody->m_aabb.Transform(model);				 // reconstruct in world space

			// updating aabb tree
			SpatialPartitionData data(rigidbody, rigidbody->m_aabb);
			m_broadPhase.UpdateData(rigidbody->m_dynamicAabbTreeKey, data);
		}
		// check for collisions
		QueryResults results;
		m_broadPhase.SelfQuery(results);
		results.DeleteDuplicates();

		// check with collision table
		for (int i = 0; i < results.m_results.size(); ++i)
		{
			QueryResult& query = results.m_results[i];
			RigidbodyComponent* rb1 = static_cast<RigidbodyComponent*>(query.m_clientData0);
			RigidbodyComponent* rb2 = static_cast<RigidbodyComponent*>(query.m_clientData1);
			if (!m_collisionTable.CheckCollisionMatrix(rb1->m_collisionMask, rb2->m_collisionMask) || 
				!m_collisionTable.CheckCollisionMatrix(rb2->m_collisionMask, rb1->m_collisionMask))
			{
				std::swap(results.m_results[i], results.m_results.back());
				results.m_results.pop_back();
			}
		}

#ifdef DEVELOPER
		m_broadPhase.DebugDraw(m_pAppRenderer,-1, Vector4(1, 0, 0, 1));
		
		for (size_t i = 0; i < results.m_results.size(); ++i)
		{
			for (size_t j = i + 1; j < results.m_results.size(); ++j)
			{
				QueryResult& r0 = results.m_results[i];
				QueryResult& r1 = results.m_results[j];

				if ((r0.m_clientData0 == r1.m_clientData0 && r0.m_clientData1 == r1.m_clientData1) ||
					(r0.m_clientData0 == r1.m_clientData1 && r0.m_clientData1 == r1.m_clientData0))
					DEBUG_LOG("RigidbodySystem::LateUpdate(): broadphase duplicate results were not properly deleted.");
			}
		}
#endif
		
#pragma endregion BroadPhaase

#pragma region NarrowPhase

		for (QueryResult& query : results.m_results)
		{
			RigidbodyComponent* rb1 = static_cast<RigidbodyComponent*>(query.m_clientData0);
			RigidbodyComponent* rb2 = static_cast<RigidbodyComponent*>(query.m_clientData1);

			TransformComponent* tr1 = rb1->GetOwner()->GetComponent<TransformComponent>();
			TransformComponent* tr2 = rb2->GetOwner()->GetComponent<TransformComponent>();
			//m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(tr1->GetPosition(), tr2->GetPosition(), Vector3(0, 1, 0));
			
			ObbSupportShape supportShape1(tr1->GetPosition(), tr1->GetScale(), tr1->GetRotationMatrix());
			ObbSupportShape supportShape2(tr2->GetPosition(), tr2->GetScale(), tr2->GetRotationMatrix());

			Gjk gjk;
			Gjk::CsoPoint closestPoint;
			float epsilon = 0.001f;
			std::vector<Gjk::CsoPoint> simplex;
			if (gjk.Intersect(simplex, &supportShape1, &supportShape2, closestPoint, epsilon, m_pAppRenderer, true))
			{
				CollisionManifold collision;
				collision.m_objectA = rb1;
				collision.m_objectB = rb2;
				if (gjk.Epa(simplex, &supportShape1, &supportShape2, collision, epsilon))
				{
					rb1->m_isColliding = true;
					rb2->m_isColliding = true;

					collision.m_objectA->m_constraints.clear();
					collision.m_objectB->m_constraints.clear();
					
					// Normal constraint for both objects
					Constraint constraintNormal(collision.m_objectA, collision.m_objectB, collision.m_depth);
					constraintNormal.CalculateNormalJacobian(collision);
					collision.m_objectA->m_constraints.push_back(constraintNormal);
					//collision.m_objectB->m_constraints.push_back(constraintNormal);

					// Friction constraint for both objects
					Constraint constraintFriction1(collision.m_objectA, collision.m_objectB);
					Constraint constraintFriction2(collision.m_objectA, collision.m_objectB);
					Constraint::CalculateFrictionJacobians(collision, constraintFriction1, constraintFriction2);
					collision.m_objectA->m_constraints.push_back(constraintFriction1);
					//collision.m_objectB->m_constraints.push_back(constraintFriction1);
					collision.m_objectA->m_constraints.push_back(constraintFriction2);
					//collision.m_objectB->m_constraints.push_back(constraintFriction2);					
				}
			}
			// else no collision
			else
			{
				//m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(closestPoint.m_PointA, closestPoint.m_PointB, Vector3(0,0,1));

				
				//float separatingDistance = Vector3::Distance(closestPoint.m_PointA, closestPoint.m_PointB);
			}
		}
		
#pragma endregion NarrowPhase

#pragma region VelocityUpdate
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;
			TransformComponent* transform = rigidbodyNode->m_transform;

			Vector3& velocity = rigidbody->m_velocity;
			Vector3& angularV = rigidbody->m_angularVelocity;

			rigidbody->m_velocityLastFrame = velocity;
			rigidbody->m_angularVelocityLastFrame = angularV;

			const Matrix& rotMat = transform->GetRotationMatrix();
			const Matrix inverseRotMat = rotMat.Transpose();
			rigidbody->m_inertiaTensorWorldInverse = rotMat * rigidbody->m_inertiaTensorInverse * inverseRotMat;

			// gravity
			Vector3 force;
			force += Vector3(0, PhysicsUtils::Consts::gravity, 0) * rigidbody->m_mass;
			
		
			if (rigidbody->m_inverseMass < PhysicsUtils::Consts::minMass)
				continue;
			// airdrag
			float airDragForCube = 1.05f;
			force -= airDragForCube * velocity;
			velocity += PhysicsUtils::Consts::fixedTimeStep * rigidbody->m_inverseMass * force;

			
			Vector3 angularAcceleration;
			Vector3 torque;// outside source FxR
			angularAcceleration = Vector3::Transform(torque, rigidbody->m_inertiaTensorWorldInverse);
			angularV += PhysicsUtils::Consts::fixedTimeStep * angularAcceleration;
		}
#pragma endregion VelocityUpdate

#pragma region SolvingConstraints
		// constraints update
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;

			std::vector<Constraint>& constraints = rigidbody->m_constraints;
			if (!constraints.empty())
			{				
				Vector3& velocity = rigidbody->m_velocity;
				float vLen = velocity.LengthSquared();
				Vector3& angularV = rigidbody->m_angularVelocity;
				
				//precomputing some of the terms
				std::vector<Jacobian> velocityVectors;
				velocityVectors.reserve(constraints.size() / 3); // 1 normal constraint + 2 friction constraint = 3 types of constraints

				std::vector<float> sumLambda(constraints.size(), 0.0f);
				std::vector<float> sumLambdaOld(constraints.size(), 0.0f);

				// Gaus-Saidel
				for (int i = 0; i < PhysicsUtils::Consts::Constraints::numGaussSeidelIterations; i++)
				{
					// solve for each constraint and update velocity
					for (int j = 0; j < constraints.size(); j++)
					{
						// calculating Velocity vector
						Vector3& v1 = constraints[j].m_object1->m_velocity;
						Vector3& w1 = constraints[j].m_object1->m_angularVelocity;
						Vector3& v2 = constraints[j].m_object2->m_velocity;
						Vector3& w2 = constraints[j].m_object2->m_angularVelocity;
						Jacobian V(v1, w1, v2, w2);

						// Jacobian * Veclocity vector
						float JV = constraints[j].m_jacobian * V;

						// calculating inverse Mass matrix
						Matrix identityMatrix;
						Matrix m1 = constraints[j].m_object1->m_inverseMass * identityMatrix;
						Matrix I1 = constraints[j].m_object1->m_inertiaTensorWorldInverse;
						Matrix m2 = constraints[j].m_object2->m_inverseMass * identityMatrix;
						Matrix I2 = constraints[j].m_object2->m_inertiaTensorWorldInverse;
						MassMatrix massInverseMatrix(m1, I1, m2, I2);

						// M^-1 * J.transposed()
						Jacobian massMatrixInverseJTranspose;
						massInverseMatrix.MultiplyByJacobian(massMatrixInverseJTranspose, constraints[j].m_jacobian);

						float jacobianMatrixDotMassMatrixInverseJTranspose = constraints[j].m_jacobian * massMatrixInverseJTranspose;
						float effectiveMass = 1.0f / jacobianMatrixDotMassMatrixInverseJTranspose;//  (constraints[j].m_jacobian* massMatrixInverseJTranspose);

						// calculate lambda
						float zeta = 0.0f;
						if (j % 3 == 0) // normal constraints
						{
							float depth = constraints[j].m_depthPen;
							const float slop = -0.005f; // penetration error
							zeta = -PhysicsUtils::Consts::Constraints::bias * (depth + slop) / PhysicsUtils::Consts::fixedTimeStep;// +
								PhysicsUtils::Consts::Constraints::restitution * JV;
						}

						float lambda = -(JV + zeta) * effectiveMass;
						//lambda += constraints[j].m_lambda;
						
						sumLambdaOld[j] = sumLambda[j];
						sumLambda[j] += lambda;

						if (j % 3 == 0) // clamp lambda for normal constraint
						{
							//MathUtil::Clamp(lambda, 0.0f, MathUtil::PositiveMax());
							MathUtil::Clamp(sumLambda[j], 0.0f, MathUtil::PositiveMax());
						}
						else // clamp lambda for friction constraint
						{
							// not sure if this is the correct way, seems like the size of each object should matter
							float contactMass = (constraints[j].m_object1->m_mass + constraints[j].m_object2->m_mass) / 2.0f;

							// dot product should give us the cos of angle between normal and gravity (both should be normalized at this point)
							float collisionWeight = PhysicsUtils::Consts::gravity * constraints[j].m_normal.Dot(Vector3(0.0f, 1.0f, 0.0f));

							MathUtil::Clamp(sumLambda[j], -PhysicsUtils::Consts::Constraints::friction * collisionWeight,
							//MathUtil::Clamp(lambda, -PhysicsUtils::Consts::Constraints::friction * collisionWeight,
								PhysicsUtils::Consts::Constraints::friction * collisionWeight);
						}
						float deltaLambda = sumLambda[j] - sumLambdaOld[j];
						//float deltaLambda = lambda - constraints[j].m_lambda;
						//constraints[j].m_lambda = lambda;

						/*if (abs(deltaLambda) < 0.001f)
						{
							std::swap(rigidbody->m_constraints[j], rigidbody->m_constraints.back());
							rigidbody->m_constraints.pop_back();
							continue;
						}*/
						// this is change in velocity or acceleration due to constraint
						Jacobian velocityCorrection;
						massMatrixInverseJTranspose.MultiplyByFloat(velocityCorrection, deltaLambda);

						Jacobian finalVel;
						V.AddJacobian(finalVel, velocityCorrection);

						// converting back to normal storage
						if (constraints[j].m_object1->m_collisionMask == CollisionTable::CollisionMask::STATIC_OBJ)
						{
							finalVel.m_velocity1 = Vector3(0.0f, 0.0f, 0.0f);
							finalVel.m_angularVelocity1 = Vector3(0.0f, 0.0f, 0.0f);
						}
						if (constraints[j].m_object2->m_collisionMask == CollisionTable::CollisionMask::STATIC_OBJ)
						{
							finalVel.m_velocity2 = Vector3(0.0f, 0.0f, 0.0f);
							finalVel.m_angularVelocity2 = Vector3(0.0f, 0.0f, 0.0f);
						}
						constraints[j].m_object1->m_velocity = finalVel.m_velocity1;
						constraints[j].m_object1->m_angularVelocity = finalVel.m_angularVelocity1;
						constraints[j].m_object2->m_velocity = finalVel.m_velocity2;
						constraints[j].m_object2->m_angularVelocity = finalVel.m_angularVelocity2;
					}
				}
				rigidbody->m_constraints.clear();
			}
		}
#pragma endregion SolvingConstraints

#pragma region PositionUpdate
		// position update
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;
			TransformComponent* transform = rigidbodyNode->m_transform;

			Vector3 position = transform->GetPosition();
			Vector3& velocity = rigidbody->m_velocity;
			
			Quaternion orientationQuat = Quaternion::CreateFromRotationMatrix(transform->GetRotationMatrix());
			/*if ((rigidbody->m_velocityLastFrame - velocity).LengthSquared() < 0.001f)
			{
				velocity = rigidbody->m_velocityLastFrame;
			}*/
			Quaternion angularVelocity(rigidbody->m_angularVelocity.x, rigidbody->m_angularVelocity.y, rigidbody->m_angularVelocity.z, 0);
			/*if ((rigidbody->m_angularVelocityLastFrame - rigidbody->m_angularVelocity).LengthSquared() < 0.001f)
			{
				rigidbody->m_angularVelocity = rigidbody->m_angularVelocityLastFrame;
			}*/
			
			position += velocity * PhysicsUtils::Consts::fixedTimeStep;
			Quaternion wq = angularVelocity * orientationQuat;
			orientationQuat += PhysicsUtils::Consts::fixedTimeStep * 0.5f * wq;

			Vector3 eularRotation = MathUtil::ToEulerAngles(orientationQuat);
			
			rigidbody->m_velocity = velocity;
			rigidbody->m_position = position;

			transform->SetLocalPosition(position);
			float x = eularRotation.x * 180 / PI;
			float y = eularRotation.y * 180 / PI;
			float z = eularRotation.z * 180 / PI;

			transform->SetLocalRotation(eularRotation.x * 180 / PI, eularRotation.y * 180 / PI, eularRotation.z * 180 / PI);
		}

		m_timeAccumulator -= PhysicsUtils::Consts::fixedTimeStep;
	}
#pragma endregion PositionUpdate
}

DynamicAabbTree& RigidbodySystem::GetAabbTree()
{
	return m_broadPhase;
}

void RigidbodySystem::OnKeyDown(const KeyEvent* keyEvent)
{
#ifdef DEVELOPER
	static bool isSpacePressedIn = false;
	static bool isRightPressedIn = false;
	switch (keyEvent->m_scancode)
	{
		case SDL_SCANCODE_SPACE:
			isSpacePressedIn = !isSpacePressedIn;
			if (isSpacePressedIn)
			{
				m_isPaused = !m_isPaused;
			}
			break;
		case SDL_SCANCODE_RIGHT:
			isRightPressedIn = !isRightPressedIn;
			if (isRightPressedIn)
			{
				m_isAdvanceOneStep = !m_isAdvanceOneStep;
			}
			break;
		default:
			break;
	}
#endif

}


