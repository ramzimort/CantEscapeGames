
#include "CameraComponent.h"
#include "Graphics/Camera.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"


unsigned const CameraComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<CameraComponent>("CameraComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("Camera", &CameraComponent::m_camera)
		.method("Init", &CameraComponent::Init);
}

CameraComponent::CameraComponent(GameObject *owner)
	: BaseComponent(owner, CameraComponent::static_type)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Init(ResourceManager* resMgr)
{
	//TODO: Add a message to set this as the main camera if it has that property

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