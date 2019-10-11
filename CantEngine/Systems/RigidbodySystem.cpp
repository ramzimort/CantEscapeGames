/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "Physics/SuppportShape/ObbSupportShape.h"
#include "RigidbodySystem.h"
#include "GameObjects/GameObject.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/Gjk/Gjk.h"
#include "CantDebug/CantDebug.h"
unsigned const RigidbodySystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "Components/TransformComponent.h"
#include "Components/RigidbodyComponent.h"

#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"


RigidbodySystem::RigidbodySystem() : 
	BaseSystem(), m_timeAccumulator(0.0f)
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<TransformComponent>();
	Push_required_comp<RigidbodyComponent>();
	Push_required_comp<MeshComponent>();
}


void RigidbodySystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	TransformComponent*transform = go->GetComponent<TransformComponent>();
	RigidbodyComponent*rigidbody = go->GetComponent<RigidbodyComponent>();
	MeshComponent*mesh = go->GetComponent<MeshComponent>();

	BaseSystemCompNode *component_node = new RigidbodyCompNode(transform, rigidbody, mesh);
	component_node->m_goID = go->GetId();


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
	//GraphicsSettings::Draw_Mesh_Flag = false;

	m_timeAccumulator += dt;
	while (m_timeAccumulator >= PhysicsUtils::Consts::fixedTimeStep)
	{
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

#ifdef DEVELOPER
		m_broadPhase.DebugDraw(m_pAppRenderer,-1, debugColor);
		
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
		
		debugColor = Vector4(0, 1, 0, 1);
		for (QueryResult& query : results.m_results)
		{

				debugColor = Vector4(1, 0, 0, 1);
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
					collision.m_objectA->m_constraints.clear();
					collision.m_objectB->m_constraints.clear();

					// Normal constraint for both objects
					Constraint constraintNormal(collision.m_objectA, collision.m_objectB, collision.m_depth);
					constraintNormal.CalculateNormalJacobian(collision);
					collision.m_objectA->m_constraints.push_back(constraintNormal);
					collision.m_objectB->m_constraints.push_back(constraintNormal);

					// Friction constraint for both objects
					Constraint constraintFriction1(collision.m_objectA, collision.m_objectB);
					Constraint constraintFriction2(collision.m_objectA, collision.m_objectB);
					Constraint::CalculateFrictionJacobians(collision, constraintFriction1, constraintFriction2);
					collision.m_objectA->m_constraints.push_back(constraintFriction1);
					collision.m_objectB->m_constraints.push_back(constraintFriction1);
					collision.m_objectA->m_constraints.push_back(constraintFriction2);
					collision.m_objectB->m_constraints.push_back(constraintFriction2);					
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

			const Matrix& rotMat = transform->GetRotationMatrix();
			const Matrix inverseRotMat = rotMat.Transpose();
			rigidbody->m_inertiaTensorWorldInverse = rotMat * rigidbody->m_inertiaTensorInverse * inverseRotMat;

			// calculate acceleration due to gravity, airdrag, sping acceleration, etc ( a = (all forces) / m )
			Vector3 acceleration;
			if (rigidbody->m_isEffectedByGravity)
				acceleration += Vector3(0, PhysicsUtils::Consts::gravity, 0);

			velocity += acceleration * PhysicsUtils::Consts::fixedTimeStep;
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

						float effectiveMass = 1.0f / (constraints[j].m_jacobian * massMatrixInverseJTranspose);

						// calculate lambda
						float zeta = 0.0f;
						if (j % 3 == 0) // normal constraints
						{
							float depth = constraints[j].m_depthPen;
							zeta = -PhysicsUtils::Consts::Constraints::bias * depth / PhysicsUtils::Consts::fixedTimeStep +
								PhysicsUtils::Consts::Constraints::restitution * JV;

						}

						float lambda = -(JV + zeta) * effectiveMass;
						sumLambdaOld[j] = sumLambda[j];
						sumLambda[j] += lambda;

						if (j % 3 == 0) // clamp lambda for normal constraint
						{
							MathUtil::Clamp(sumLambda[j], 0.0f, MathUtil::PositiveMax());
						}
						else // clamp lambda for friction constraint
						{
							// not sure if this is the correct way, seems like the size of each object should matter
							float contactMass = (constraints[j].m_object1->m_mass + constraints[j].m_object2->m_mass) / 2.0f;

							// dot product should give us the cos of angle between normal and gravity (both should be normalized at this point)
							float collisionWeight = PhysicsUtils::Consts::gravity * constraints[j].normal.Dot(Vector3(0.0f, 1.0f, 0.0f));

							MathUtil::Clamp(sumLambda[j], -PhysicsUtils::Consts::Constraints::friction * collisionWeight,
								PhysicsUtils::Consts::Constraints::friction * collisionWeight);
						}

						float deltaLambda = sumLambda[j] - sumLambdaOld[j];

						// this is change in velocity or acceleration due to constraint
						Jacobian velocityCorrection;
						massMatrixInverseJTranspose.MultiplyByFloat(velocityCorrection, deltaLambda);

						Jacobian finalVel;
						V.AddJacobian(finalVel, velocityCorrection);

						// converting back to normal storage
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
			Quaternion angularVelocity(rigidbody->m_angularVelocity.x, rigidbody->m_angularVelocity.y, rigidbody->m_angularVelocity.z, 0);

			position += velocity * PhysicsUtils::Consts::fixedTimeStep;
			Quaternion wq = angularVelocity * orientationQuat;
			rigidbody->m_quaternion += PhysicsUtils::Consts::fixedTimeStep * 0.5f * wq;

			Vector3 eularRotation = MathUtil::ToEulerAngles(rigidbody->m_quaternion);
			
			rigidbody->m_velocity = velocity;
			rigidbody->m_position = position;

			transform->SetLocalPosition(position);
			transform->SetLocalRotation(eularRotation.x, eularRotation.y, eularRotation.z);
		}

		m_timeAccumulator -= PhysicsUtils::Consts::fixedTimeStep;
	}
#pragma endregion PositionUpdate
}

DynamicAabbTree& RigidbodySystem::GetAabbTree()
{
	return m_broadPhase;
}



