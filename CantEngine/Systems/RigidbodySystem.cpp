/**
 * @file RigidbodySystem.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#include "Physics/SuppportShape/ObbSupportShape.h"
#include "Physics/SuppportShape/ModelSupportShape.h"
#include "RigidbodySystem.h"
#include "GameObjects/GameObject.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/Gjk/Gjk.h"
#include "CantDebug/CantDebug.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"
#include "Components/TransformComponent.h"
#include "Components/RigidbodyComponent.h"

unsigned const RigidbodySystem::static_type = BaseSystem::numberOfTypes++;

RigidbodySystem::RigidbodySystem() : 
	BaseSystem(), m_timeAccumulator(0.0f)
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<RigidbodyComponent>();
	Push_required_comp<MeshComponent>();

#ifdef DEVELOPER
	EventManager::Get()->SubscribeEvent<KeyEvent>(this,
		std::bind(&RigidbodySystem::OnKeyDown, this, std::placeholders::_1));
#endif
	EventManager::Get()->SubscribeEvent<GameObjectDestroyed>(this,
		std::bind(&RigidbodySystem::OnObjectDeleted, this, std::placeholders::_1));
}

RigidbodySystem::~RigidbodySystem()
{
#ifdef DEVELOPER
	EventManager::Get()->UnsubscribeEvent<KeyEvent>(this);
#endif
	EventManager::Get()->UnsubscribeEvent<GameObjectDestroyed>(this);
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
	//while (m_timeAccumulator >= PhysicsUtils::Consts::fixedTimeStep)
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

			rigidbody->m_isColliding = false;
			
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
				--i; // to not skip the swapped value
			}
		}

#ifdef DEVELOPER
		if(PhysicsUtils::Settings::Draw_Dynamic_AABB_Tree)
			m_broadPhase.DebugDraw(m_pAppRenderer, PhysicsUtils::Settings::dynamicAabbLevelDraw, Vector4(1, 0, 0, 1));
		
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

#pragma region ValidateContacts
		for (int j = 0; j < m_contactManifolds.size(); ++j)//ContactManifold& contactManifold : m_contactManifolds)
		{
			std::vector<Contact>& contacts = m_contactManifolds[j].m_contacts;
			for (int i = 0; i < contacts.size(); ++i)//Contact& contact : contactManifold.m_contacts)
			{
				TransformComponent* trA = contacts[i].m_objectA->GetOwner()->GetComponent<TransformComponent>();
				TransformComponent* trB = contacts[i].m_objectB->GetOwner()->GetComponent<TransformComponent>();

				const Matrix modelToWorldA = trA->GetModel();
				const Matrix modelToWorldB = trB->GetModel();

				const Vector3 newGlobalA = Vector3::Transform(contacts[i].m_pALocal, modelToWorldA);
				const Vector3 newGlobalB = Vector3::Transform(contacts[i].m_pBLocal, modelToWorldB);

				const Vector3 rAB = newGlobalB - newGlobalA;
				const Vector3 rA = contacts[i].m_pA - newGlobalA;
				const Vector3 rB = contacts[i].m_pB - newGlobalB;

				const float epsilon = PhysicsUtils::Consts::penetrationEpsilon;

				const bool isStillPenetrating = contacts[i].m_normal.Dot(rAB) <= 0.0f;
				const bool isAcloseEnough = rA.LengthSquared() < epsilon;
				const bool isBcloseEnough = rB.LengthSquared() < epsilon;

				if (isStillPenetrating &&
					isAcloseEnough &&
					isBcloseEnough)
				{
					// keep the contact
				}
				else
				{
					std::swap(contacts[i], contacts.back());
					contacts.pop_back();
					--i; // to not skip swapped value
				}
			}
			//if (contacts.size() == 0)
			//{
			//	std::swap(m_contactManifolds[j], m_contactManifolds.back());
			//	m_contactManifolds.pop_back();
			//	--j; // to not skip swapped value
			//}
		}
#pragma endregion

#pragma region NarrowPhase

		for (QueryResult& query : results.m_results)
		{
			RigidbodyComponent* rb1 = static_cast<RigidbodyComponent*>(query.m_clientData0);
			RigidbodyComponent* rb2 = static_cast<RigidbodyComponent*>(query.m_clientData1);

			TransformComponent* tr1 = rb1->GetOwner()->GetComponent<TransformComponent>();
			TransformComponent* tr2 = rb2->GetOwner()->GetComponent<TransformComponent>();

			MeshComponent* mesh1 = rb1->GetOwner()->GetComponent<MeshComponent>();
			MeshComponent* mesh2 = rb2->GetOwner()->GetComponent<MeshComponent>();

			Contact contact;
			contact.m_objectA = rb1;
			contact.m_objectB = rb2;
			const Aabb& localAabb1 = mesh1->GetModel()->GetAABB();
			const Aabb& localAabb2 = mesh2->GetModel()->GetAABB();
			ObbSupportShape modelSupportA = ObbSupportShape(tr1->GetPosition(), tr1->GetScale(), tr1->GetRotationMatrix(), localAabb1);
			ObbSupportShape modelSupportB = ObbSupportShape(tr2->GetPosition(), tr2->GetScale(), tr2->GetRotationMatrix(), localAabb2);
			//ModelSupportShape modelSupportA(*(mesh1->GetModel()), *tr1);
			//ModelSupportShape modelSupportB(*(mesh2->GetModel()), *tr2);
			Gjk gjk;
			const float epsilon = 0.001f;
			std::vector<Gjk::CsoPoint> simplex;

			Gjk::CsoPoint closestPoint;
			
			if (gjk.Intersect(simplex, &modelSupportA, &modelSupportB, closestPoint, epsilon, m_pAppRenderer, true))
			{
				if (gjk.Epa(simplex, &modelSupportA, &modelSupportB, contact, m_pAppRenderer, epsilon))
				{
					// collision point in local space
					contact.m_pALocal = PhysicsUtils::WorldToModel(tr1->GetModel(), contact.m_pA);
					contact.m_pBLocal = PhysicsUtils::WorldToModel(tr2->GetModel(), contact.m_pB);
				
					rb1->m_isColliding = true;
					rb2->m_isColliding = true;

					ProcessCollision(contact);
				}
			}
		}
#pragma endregion NarrowPhase

		// debugdraw of contacts
#ifdef DEVELOPER
		if (PhysicsUtils::Settings::isDrawContactPoints)
		{
			for (const ContactManifold& contactManifold : m_contactManifolds)
			{
				for (const Contact& contact : contactManifold.m_contacts)
				{
					if (contact.m_constraints.size() < 3)
						continue;
					DebugAABBInstance debugAabb;
					debugAabb.m_min_bound = contact.m_pA - Vector3(0.1f);
					debugAabb.m_max_bound = contact.m_pA + Vector3(0.1f);
					debugAabb.m_color = Vector3(0.0f, 0.0f, 1.0f);

					m_pAppRenderer->GetDebugRendering().RegisterDebugAABB(debugAabb);

					Vector3 u1; // tangent to normal, direction unit vector for friction in 1 of the directions
					Vector3 u2; // bitangent to normal, direction unit vector for friction in 2 of the directions
					MathUtil::BuildTangentBitangent(contact.m_normal, u1, u2);
					m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(contact.m_pB, contact.m_pB + contact.m_constraints[0].m_jacobian.m_angularVelocity2, Vector3(1, 0, 0));
					m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(contact.m_pB, contact.m_pB + contact.m_constraints[1].m_jacobian.m_angularVelocity2, Vector3(0, 1, 0));
					m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(contact.m_pB, contact.m_pB + contact.m_constraints[2].m_jacobian.m_angularVelocity2, Vector3(0, 0, 1));
				}
			}
		}
#endif

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

			if (rigidbody->m_inverseMass < PhysicsUtils::Consts::minMass)
				continue;
			Vector3 force;
			// gravity
			if (rigidbody->m_isEffectedByGravity)
				force += Vector3(0, PhysicsUtils::Consts::gravity, 0) * rigidbody->m_mass;
			
				// airdrag
			float airDragForCube = 1.05f;
			force -= airDragForCube * velocity;
			velocity += PhysicsUtils::Consts::fixedTimeStep * rigidbody->m_inverseMass * force;
			

			Vector3 angularAcceleration;
			Vector3 torque;// outside source FxR
			angularAcceleration = Vector3::Transform(torque, rigidbody->m_inertiaTensorWorldInverse);
			//angularV += PhysicsUtils::Consts::fixedTimeStep * angularAcceleration;
		}
#pragma endregion VelocityUpdate

#pragma region SolvingConstraints
		// Gaus-Saidel
		// for each collision with another object
		for (ContactManifold& contactManifold : m_contactManifolds)
		{
			// notifying that collision happened between two objects
			if (contactManifold.m_contacts.size() > 0)
			{
				RigidbodyComponent* rb1 = contactManifold.m_object1;
				RigidbodyComponent* rb2 = contactManifold.m_object2;
				rb1->m_onCollision(rb1->GetOwner(), rb2->GetOwner());
				rb2->m_onCollision(rb2->GetOwner(), rb1->GetOwner());
			}

			const int numIterations = 50;// PhysicsUtils::Consts::Constraints::numGaussSeidelIterations
			for (int i = 0; i < numIterations; i++)
			{
				std::vector<Contact>& contacts = contactManifold.m_contacts;
				// solve for each constraint and update velocity
				for (int k = 0; k < contacts.size(); k++)
				{
					if (contacts[k].m_objectA->m_collisionMask == CollisionTable::CollisionMask::STATIC_OBJ &&
						contacts[k].m_objectB->m_collisionMask == CollisionTable::CollisionMask::STATIC_OBJ )
					{
						continue;
					}
					std::vector<Constraint>& constraints = contacts[k].m_constraints;
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
						float effectiveMass = 1.0f / jacobianMatrixDotMassMatrixInverseJTranspose;
						
						// calculate lambda
						float zeta = 0.0f;
						if (j % 3 == 0) // normal constraints
						{
							float depth = constraints[j].m_depthPen;
							const float slop = 0.008f; // penetration error
							float bias = PhysicsUtils::Consts::Constraints::bias;
							//float bias = 0.0f;
							zeta = -bias * std::max((depth - slop), 0.0f) / PhysicsUtils::Consts::fixedTimeStep;// +
								//PhysicsUtils::Consts::Constraints::restitution * JV; // TODO: add this when stable
						}

						float lambda = -(JV + zeta) * effectiveMass;
						lambda += constraints[j].m_lambda;

						if (j % 3 == 0) // clamp lambda for normal constraint
						{
							lambda = MathUtil::Clamp(lambda, 0.0f, MathUtil::PositiveMax());
						}
						else // clamp lambda for friction constraint
						{
							// not sure if this is the correct way, seems like the size of each object should matter
							//float contactMass = (constraints[j].m_object1->m_mass + constraints[j].m_object2->m_mass) / 2.0f;

							// dot product should give us the cos of angle between normal and gravity (both should be normalized at this point)
							//float collisionWeight = PhysicsUtils::Consts::gravity * constraints[j].m_normal.Dot(Vector3(0.0f, 1.0f, 0.0f));
							//lambda = MathUtil::Clamp(lambda, -PhysicsUtils::Consts::Constraints::friction * collisionWeight, PhysicsUtils::Consts::Constraints::friction * collisionWeight);
							float averageFrction = (constraints[j].m_object1->m_frictionCoef + constraints[j].m_object2->m_frictionCoef) * 0.5f;
							float maxFrictionMultiplier = averageFrction * constraints[0].m_lambda;
							lambda = MathUtil::Clamp(lambda, -maxFrictionMultiplier, maxFrictionMultiplier);
						}

						float deltaLambda = lambda - constraints[j].m_lambda;
						constraints[j].m_lambda = lambda;

						Jacobian velocityCorrection;
						massMatrixInverseJTranspose.MultiplyByFloat(velocityCorrection, deltaLambda);

						Jacobian finalVel;
						V.AddJacobian(finalVel, velocityCorrection);

						constraints[j].m_object1->m_velocity = finalVel.m_velocity1;
						//constraints[j].m_object1->m_angularVelocity = finalVel.m_angularVelocity1;
						constraints[j].m_object2->m_velocity = finalVel.m_velocity2;
						//constraints[j].m_object2->m_angularVelocity = finalVel.m_angularVelocity2;
					}
				}
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
			
			float angle = PhysicsUtils::Consts::pi_4 / PhysicsUtils::Consts::fixedTimeStep;//rigidbody->m_angularVelocity.Length();
			float epsilon = 0.001f;
			Vector3 axis;
			if (angle * PhysicsUtils::Consts::fixedTimeStep > PhysicsUtils::Consts::pi_4)
			{
				angle = PhysicsUtils::Consts::pi_4 / PhysicsUtils::Consts::fixedTimeStep;
			}
			else if (angle < epsilon)
			{
				 // from Bullit
				axis = rigidbody->m_angularVelocity * 0.5f * PhysicsUtils::Consts::fixedTimeStep 
					- PhysicsUtils::Consts::fixedTimeStep * PhysicsUtils::Consts::fixedTimeStep * PhysicsUtils::Consts::fixedTimeStep
					* Vector3(0.20833333333f) * angle * angle;
			}
			else
			{
				axis = rigidbody->m_angularVelocity * sinf(0.5f * angle * PhysicsUtils::Consts::fixedTimeStep) / angle;
			}
			
			Quaternion deltaOrientation(axis, cosf(angle * PhysicsUtils::Consts::fixedTimeStep * 0.5f));

			//Vector3 deltaAngularVelocity = PhysicsUtils::Consts::fixedTimeStep * (rigidbody->m_angularVelocity);

			Quaternion orientationQuat = Quaternion::CreateFromRotationMatrix(transform->GetRotationMatrix());
			//Quaternion angularVelocity(deltaAngularVelocity.x, deltaAngularVelocity.y, deltaAngularVelocity.z, 0);
			
			position += velocity * PhysicsUtils::Consts::fixedTimeStep;
			Quaternion wq = deltaOrientation * orientationQuat;
			wq.Normalize();
			orientationQuat = wq;

			Vector3 eularRotation = MathUtil::ToEulerAngles(orientationQuat);
			
			rigidbody->m_velocity = velocity;
			rigidbody->m_position = position;

			transform->SetLocalPosition(position);

			MathUtil::RadiansToDegree(eularRotation);
			//transform->SetLocalRotation(eularRotation.x, eularRotation.y, eularRotation.z);
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
		case SDL_SCANCODE_LEFTBRACKET:
			isSpacePressedIn = !isSpacePressedIn;
			if (isSpacePressedIn)
			{
				m_isPaused = !m_isPaused;
			}
			break;
		case SDL_SCANCODE_RIGHTBRACKET:
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


void RigidbodySystem::ProcessCollision(Contact& contact)
{
	for (ContactManifold& contactManifold : m_contactManifolds)
	{
		if ((contactManifold.m_object1 == contact.m_objectA && contactManifold.m_object2 == contact.m_objectB) ||
			(contactManifold.m_object1 == contact.m_objectB && contactManifold.m_object2 == contact.m_objectA))
		{
			contactManifold.ProccessCollision(contact);
			contactManifold.KeepOnlyFourContacts();

			return;
		}
	}

	// else colliding with a new body
	contact.BuildConstraints();
	ContactManifold contactManifold(contact);
	m_contactManifolds.push_back(contactManifold); // creates new contact manifold created implicitly from collision manifold
}

void RigidbodySystem::RemoveCollision(const Contact& collision)
{
	for (int i = 0; i < m_contactManifolds.size(); ++i)
	{
		if ((m_contactManifolds[i].m_object1 == collision.m_objectA && m_contactManifolds[i].m_object2 == collision.m_objectB) ||
			(m_contactManifolds[i].m_object1 == collision.m_objectB && m_contactManifolds[i].m_object2 == collision.m_objectA))
		{
			std::swap(m_contactManifolds[i], m_contactManifolds.back());
			m_contactManifolds.pop_back();
			return;
		}
	}
}

void RigidbodySystem::OnObjectDeleted(const GameObjectDestroyed* e)
{
	RigidbodyComponent* rigidbody = e->m_pGameObject->GetComponent<RigidbodyComponent>();
	if (rigidbody)
		m_broadPhase.RemoveData(rigidbody->m_dynamicAabbTreeKey);
}