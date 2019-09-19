/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "RenderingSystem.h"
#include "../GameObjects/GameObject.h"
unsigned const RenderingSystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../Components/TransformComponent.h"
#include "../Components/RendererComponent.h"


RenderingSystem::RenderingSystem() : 
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<Transform>();
	Push_required_comp<Renderer>();
}


void RenderingSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	Transform *transform = go->GetComponent<Transform>();
	Renderer *renderer = go->GetComponent<Renderer>();

	BaseSystemCompNode *component_node = new RenderingCompNode(transform, renderer);
	component_node->go_id = go->GetId();

	this->go_components_map[go->GetId()] = component_node;
}


void RenderingSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RenderingCompNode *node = static_cast<RenderingCompNode*>(compNode);
	Transform *T = node->n_transform;
	Renderer *R = node->n_renderer;

	//UPDATE CODE GOES HERE
}