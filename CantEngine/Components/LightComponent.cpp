
#include "LightComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

unsigned const LightComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<LightComponent>("LightComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("Light", &LightComponent::m_light)
		.method("Init", &LightComponent::Init);

}

LightComponent::LightComponent(GameObject *owner)
	:BaseComponent(owner, static_type)
{
}


LightComponent::~LightComponent()
{
}


const Light* LightComponent::GetLight() const
{
	return &m_light;
}

void LightComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{

}

void LightComponent::Begin(GameObjectManager *goMgr)
{

}