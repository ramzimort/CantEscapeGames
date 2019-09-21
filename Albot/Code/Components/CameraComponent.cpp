#include "CameraComponent.h"


unsigned const CameraComponent::static_type = BaseComponent::numberOfTypes++;

CameraComponent::CameraComponent(GameObject *owner, ComponentId type)
	:BaseComponent(owner, CameraComponent::static_type)
{
}


CameraComponent::~CameraComponent()
{
}


Camera* CameraComponent::GetCamera() const
{
	return m_camera;
}