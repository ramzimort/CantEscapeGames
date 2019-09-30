/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "RigidbodySystem.h"
#include "../GameObjects/GameObject.h"
#include "Physics/PhysicsUtils.h"
#include "CantDebug/CantDebug.h"
unsigned const RigidbodySystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"


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


//This one is not called one per object in the system, but once per system.
//Access all the components data and stuff through the unordered_map
void RigidbodySystem::LateUpdate(float dt) 
{
	m_timeAccumulator += dt;
	while (m_timeAccumulator >= PhysicsUtils::Consts::fixedTimeStep)
	{
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
		m_broadPhase.DebugDraw(-1, Vector4(1, 1, 1, 1));
#endif

		
#ifdef DEVELOPER
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

		// TODO: narrow phase (SAT)
		
		// movement update
		for (auto& node : m_ObjComponentsMap)
		{
			RigidbodyCompNode* rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);
			RigidbodyComponent* rigidbody = rigidbodyNode->m_rigidbody;
			TransformComponent* transform = rigidbodyNode->m_transform;

			Vector3 velocity = rigidbody->m_velocity;
			Vector3 position = transform->GetPosition();

			Vector3 acceleration = PhysicsUtils::Consts::gravity;

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



