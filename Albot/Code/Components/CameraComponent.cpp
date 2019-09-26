#include "CameraComponent.h"
#include "Graphics/Camera.h"

unsigned const CameraComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<CameraComponent>("CameraComponent")
		.property("Camera", &CameraComponent::m_camera)
		.method("Init", &CameraComponent::Init)
		.method("Begin", &CameraComponent::Begin);
}

CameraComponent::CameraComponent(GameObject *owner)
	: BaseComponent(owner, CameraComponent::static_type)
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