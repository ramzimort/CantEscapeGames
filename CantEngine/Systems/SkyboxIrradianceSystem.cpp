#include "stdafx.h"
#include "SkyboxIrradianceSystem.h"

#include "GameObjects/GameObject.h"
unsigned const SkyboxIrradianceSystem::static_type = BaseSystem::numberOfTypes++;

#include "Components/TransformComponent.h"
#include "Components/SkyboxIrradianceComponent.h"
#include "Graphics/AppRenderer.h"

SkyboxIrradianceSystem::SkyboxIrradianceSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<SkyboxIrradianceComponent>();
}


SkyboxIrradianceSystem::~SkyboxIrradianceSystem()
{
}



void SkyboxIrradianceSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	SkyboxIrradianceComponent *skyboxIrradianceComp = go->GetComponent<SkyboxIrradianceComponent>();
	

	BaseSystemCompNode *component_node = new SkyboxIrradianceCompNode(transform, skyboxIrradianceComp);
	component_node->m_goID = go->GetId();
	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void SkyboxIrradianceSystem::Update(float dt, BaseSystemCompNode *compNode)
{

}

void SkyboxIrradianceSystem::Draw(float dt, BaseSystemCompNode *compNode)
{

}

void SkyboxIrradianceSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_appRenderer = appRenderer;
}