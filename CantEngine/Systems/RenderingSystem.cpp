/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "RenderingSystem.h"
#include "GameObjects/GameObject.h"
unsigned const RenderingSystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "Components/TransformComponent.h"
#include "Components/RendererComponent.h"
#include "Components/MeshComponent.h"
#include "Components/AnimationComponent.h"


#include "Graphics/AppRenderer.h"
#include "Graphics/Models/Model.h"

RenderingSystem::RenderingSystem() : 
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<TransformComponent>();
	Push_required_comp<RendererComponent>();
	Push_required_comp<MeshComponent>();
}


void RenderingSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	TransformComponent *transform = go->GetComponent<TransformComponent>();
	RendererComponent *renderer = go->GetComponent<RendererComponent>();
	MeshComponent* meshesComp = go->GetComponent<MeshComponent>();

	BaseSystemCompNode *component_node = new RenderingCompNode(transform, renderer, meshesComp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void RenderingSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RenderingCompNode *node = static_cast<RenderingCompNode*>(compNode);
	TransformComponent *T = node->m_transform;
	RendererComponent *R = node->m_renderer;

	//UPDATE CODE GOES HERE
}


void RenderingSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	RenderingCompNode *node = static_cast<RenderingCompNode*>(compNode);
	RendererComponent *rendererComp = node->m_renderer;
	if (!rendererComp->m_enableRendering)
	{
		return;
	}
	TransformComponent *transformComp = node->m_transform;
	MeshComponent* meshesComp = node->m_meshesComponent;

	const Matrix& modelMatrix = transformComp->GetModel();
	const Matrix& rotMatrix = transformComp->GetRotationMatrix();
	const Matrix& invertScaleMatrix = transformComp->GetScaleMatrix().Invert();

	//learnopengl way for getting the correct normal matrix
	//Normal = mat3(transpose(inverse(model))) * aNormal;  

	BoneMeshInstanceRenderData boneMeshInstanceRenderData = {};
	boneMeshInstanceRenderData.m_instanceRenderData = {};
	boneMeshInstanceRenderData.m_instanceRenderData.model_mat = modelMatrix;
	boneMeshInstanceRenderData.m_instanceRenderData.normal_mat = rotMatrix;
	//boneMeshInstanceRenderData.m_instanceRenderData.normal_mat =  rotMatrix.Transpose() * invertScaleMatrix;
	//boneMeshInstanceRenderData.m_instanceRenderData.normal_mat = invertScaleMatrix * rotMatrix;
	//boneMeshInstanceRenderData.m_instanceRenderData.normal_mat = modelMatrix.Invert().Transpose();
	boneMeshInstanceRenderData.m_instanceRenderData.p_ref_model = meshesComp->GetModel();
	boneMeshInstanceRenderData.m_instanceRenderData.p_ref_material = rendererComp->m_pMaterial;
	boneMeshInstanceRenderData.m_instanceRenderData.uv_tiling = Vector2(rendererComp->m_xTileFactor, rendererComp->m_yTileFactor);

	if (transformComp->GetOwner()->HasComponent<AnimationComponent>())
	{
		boneMeshInstanceRenderData.m_pBoneTransformationsList = &meshesComp->BoneTransformationsForShader;
		m_pAppRenderer->RegisterBoneMeshInstance(boneMeshInstanceRenderData);
	}
	else
	{
		m_pAppRenderer->RegisterBasicInstance(boneMeshInstanceRenderData.m_instanceRenderData);
	}
}

void RenderingSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}
