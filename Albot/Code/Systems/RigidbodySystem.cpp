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
	component_node->go_id = go->GetId();

	this->go_components_map[go->GetId()] = component_node;
}


void RigidbodySystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RigidbodyCompNode *node = static_cast<RigidbodyCompNode*>(compNode);
	Transform *T = node->n_transform;
	Rigidbody *R = node->n_rigidbody;

	//UPDATE CODE GOES HERE
}