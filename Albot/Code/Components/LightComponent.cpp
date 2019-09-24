#include "LightComponent.h"

unsigned const LightComponent::static_type = BaseComponent::numberOfTypes++;

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