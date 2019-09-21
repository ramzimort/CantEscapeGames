/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "RigidbodySystem.h"
#include "../GameObjects/GameObject.h"
unsigned const RigidbodySystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"


RigidbodySystem::RigidbodySystem() :
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<Transform>();
	Push_required_comp<Rigidbody>();
}


void RigidbodySystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	Transform *transform = go->GetComponent<Transform>();
	Rigidbody *rigidbody = go->GetComponent<Rigidbody>();

	BaseSystemCompNode *component_node = new RigidbodyCompNode(transform, rigidbody);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


//This one is not called one per object in the system, but once per system.
//Access all the components data and stuff through the unordered_map
void RigidbodySystem::LateUpdate(float dt) 
{
	for (auto& node : m_ObjComponentsMap)
	{
		RigidbodyCompNode *rigidbodyNode = static_cast<RigidbodyCompNode*>(node.second);

		Rigidbody *RB = rigidbodyNode->n_rigidbody;
		Transform *T = rigidbodyNode->n_transform;

		//ADD CODE HERE
	}
}


