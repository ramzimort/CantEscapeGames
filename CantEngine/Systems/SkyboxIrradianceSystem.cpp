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
	SkyboxIrradianceCompNode* skyboxCompNode = static_cast<SkyboxIrradianceCompNode*>(compNode);
	
	SkyboxIrradianceComponent* skyboxIrradianceComp = skyboxCompNode->m_skyboxIrradianceComp;
	TransformComponent* transformComp = skyboxCompNode->m_transform;


	BakedSkyboxIrradianceInstanceData bakedData = {};
	bakedData.m_pSkyboxTexture = skyboxIrradianceComp->m_pSkyboxTexture;
	bakedData.m_pSkyboxIrradianceTexture = skyboxIrradianceComp->m_pSkyboxIrradianceTexture;
	bakedData.m_pIBLPrefilteredEnvMapTexture = skyboxIrradianceComp->m_pIBLPrefilteredEnvMapTexture;

	if (skyboxIrradianceComp->m_hasBakedData)
	{
		m_appRenderer->RegisterBakedSkyboxIrradianceInstance(bakedData);
	}
	else
	{
		ProcessSkyboxIrradianceInstanceData processSkyboxData = {};
		processSkyboxData.m_pSkyboxEquirectangularTexture = skyboxIrradianceComp->m_pSkyboxEquirectangularTexture;
		processSkyboxData.m_pSkyboxEquirectangularIrradianceTexture = skyboxIrradianceComp->m_pSkyboxEquirectangularIrradianceTexture;
		processSkyboxData.m_hasBaked = &skyboxIrradianceComp->m_hasBakedData;
		processSkyboxData.m_bakedData = bakedData;
		m_appRenderer->RegisterProcessSkyboxIrradianceInstance(processSkyboxData);
	}

}

void SkyboxIrradianceSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_appRenderer = appRenderer;
}