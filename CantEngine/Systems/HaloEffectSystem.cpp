#include "stdafx.h"
#include "HaloEffectSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/HaloEffectComponent.h"
#include "Components/TransformComponent.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/InstanceRenderData.h"

unsigned const HaloEffectSystem::static_type = BaseSystem::numberOfTypes++;

HaloEffectSystem::HaloEffectSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<HaloEffectComponent>();
}


HaloEffectSystem::~HaloEffectSystem()
{
}


void HaloEffectSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	HaloEffectComponent* halo_comp = go->GetComponent<HaloEffectComponent>();

	BaseSystemCompNode *component_node = new HaloEffectCompNode(transform, halo_comp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void HaloEffectSystem::Update(float dt, BaseSystemCompNode *compNode)
{

}

void HaloEffectSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	HaloEffectCompNode *node = static_cast<HaloEffectCompNode*>(compNode);
	TransformComponent *transformComp = node->m_transform;
	HaloEffectComponent* haloComp = node->m_haloComp;

	HaloEffectInstanceData haloEffectInstanceData = {};
	haloEffectInstanceData.haloEffect = &haloComp->m_haloEffect;
	haloEffectInstanceData.m_haloPosition = transformComp->GetPosition() + haloComp->m_haloEffect.m_position;

	m_pAppRenderer->RegisterHaloEffectInstance(haloEffectInstanceData);
}

void HaloEffectSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}