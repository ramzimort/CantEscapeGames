#include "stdafx.h"
#include "SplineCurvesComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<SplineCurvesComponent>("SplineCurvesComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &SplineCurvesComponent::Init);
}

unsigned const SplineCurvesComponent::static_type = BaseComponent::numberOfTypes++;

SplineCurvesComponent::SplineCurvesComponent(GameObject* owner)
	:BaseComponent(owner, static_type)
{
}


SplineCurvesComponent::~SplineCurvesComponent()
{
}

void SplineCurvesComponent::Begin(GameObjectManager *goMgr)
{

}

void SplineCurvesComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{

}
