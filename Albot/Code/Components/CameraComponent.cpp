#include "CameraComponent.h"


unsigned const CameraComponent::static_type = BaseComponent::numberOfTypes++;

CameraComponent::CameraComponent(GameObject *owner)
	:BaseComponent(owner, CameraComponent::static_type)
{
}


CameraComponent::~CameraComponent()
{
}


void CameraComponent::Init()
{

}

void CameraComponent::Begin()
{

}

Camera* CameraComponent::GetCamera() const
{
	return m_camera;
}

void CameraComponent::SetCamera(Camera* camera)
{
	m_camera = camera;
}