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
#include "Components/MeshesComponent.h"

//TODO: Alberto stuff
#include "Graphics/AppRenderer.h"
extern AppRenderer* gAppRenderer;


RenderingSystem::RenderingSystem() : 
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<Transform>();
	Push_required_comp<RendererComponent>();
	Push_required_comp<MeshesComponent>();
}


void RenderingSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	Transform *transform = go->GetComponent<Transform>();
	RendererComponent *renderer = go->GetComponent<RendererComponent>();
	MeshesComponent* meshesComp = go->GetComponent<MeshesComponent>();

	BaseSystemCompNode *component_node = new RenderingCompNode(transform, renderer, meshesComp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void RenderingSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RenderingCompNode *node = static_cast<RenderingCompNode*>(compNode);
	Transform *T = node->m_transform;
	RendererComponent *R = node->m_renderer;

	//UPDATE CODE GOES HERE
}


void RenderingSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RenderingCompNode *node = static_cast<RenderingCompNode*>(compNode);
	Transform *transformComp = node->m_transform;
	RendererComponent *rendererComp = node->m_renderer;
	MeshesComponent* meshesComp = node->m_meshesComponent;

	const Matrix& modelMatrix = transformComp->GetModel();
	const Matrix& rotMatrix = transformComp->GetRotationMatrix();
	const Matrix& invertScaleMatrix = transformComp->GetScaleMatrix().Invert();

	InstanceRenderData instanceRenderData = {};
	instanceRenderData.model_mat = modelMatrix;
	instanceRenderData.normal_mat = invertScaleMatrix * rotMatrix;
	instanceRenderData.p_ref_model = meshesComp->GetModel();
	instanceRenderData.p_ref_material = rendererComp->m_pMaterial;
	instanceRenderData.uv_tiling = Vector2(rendererComp->m_x_tile_factor, rendererComp->m_y_tile_factor);

	gAppRenderer->RegisterBasicInstance(instanceRenderData);

	//DRAW CODE GOES HERE
}