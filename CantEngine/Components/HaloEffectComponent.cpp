#include "stdafx.h"
#include "HaloEffectComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"


RTTR_REGISTRATION
{
	rttr::registration::class_<HaloEffectComponent>("HaloEffectComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("HaloEffect", &HaloEffectComponent::m_haloEffect);
}

unsigned const HaloEffectComponent::static_type = BaseComponent::numberOfTypes++;

HaloEffectComponent::HaloEffectComponent(GameObject *owner)
	:BaseComponent(owner, static_type)
{
}


HaloEffectComponent::~HaloEffectComponent()
{
}


void HaloEffectComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{

}

void HaloEffectComponent::Begin(GameObjectManager *goMgr)
{

}