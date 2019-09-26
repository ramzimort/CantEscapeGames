#include "LightComponent.h"

unsigned const LightComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<LightComponent>("LightComponent")
		.property("Light", &LightComponent::m_light)
		.method("Init", &LightComponent::Init)
		.method("Begin", &LightComponent::Begin);
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

void LightComponent::Init()
{

}

void LightComponent::Begin()
{

}