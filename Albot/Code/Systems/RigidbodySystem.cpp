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
		m_broadPhase.DebugDraw(m_pAppRenderer,-1, Vector4(1, 1, 1, 1));
		
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

			TransformComponent* tr1 = rb1->GetGameObjectOwner()->GetComponent<TransformComponent>();
			TransformComponent* tr2 = rb2->GetGameObjectOwner()->GetComponent<TransformComponent>();
			
			ObbSupportShape supportShape1(tr1->GetPosition(), tr1->GetScale(), tr1->GetRotationMatrix());
			ObbSupportShape supportShape2(tr2->GetPosition(), tr2->GetScale(), tr2->GetRotationMatrix());

			Gjk gjk;
			Gjk::CsoPoint closestPoint;
			float epsilon = 0.001f;
			std::vector<Gjk::CsoPoint> simplex;
			if (gjk.Intersect(simplex, &supportShape1, &supportShape2, closestPoint, epsilon/*, PhysicsUtils::Consts::GjkGlobals::debuggingIndex, PhysicsUtils::Consts::GjkGlobals::isDebugDraw*/))
			{
				CollisionManifold collision;
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
		}
		
#pragma endregion NarrowPhase
		
		// movement update
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;
			TransformComponent* transform = rigidbodyNode->m_transform;

			Vector3 position = transform->GetPosition();
			Quaternion quat = Quaternion::CreateFromRotationMatrix(transform->GetRotationMatrix());
			Vector3 velocity = rigidbody->m_velocity;

			Vector3 acceleration;
			if (rigidbody->m_isEffectedByGravity)
				acceleration += PhysicsUtils::Consts::gravity;

			velocity += acceleration * PhysicsUtils::Consts::fixedTimeStep;

			// TODO: constraints application

			position += velocity * PhysicsUtils::Consts::fixedTimeStep;

			rigidbody->m_velocity = velocity;
			rigidbody->m_position = position;

			transform->SetLocalPosition(position);
		}
			   		
		m_timeAccumulator -= PhysicsUtils::Consts::fixedTimeStep;
	}
}

DynamicAabbTree& RigidbodySystem::GetAabbTree()
{
	return m_broadPhase;
}



