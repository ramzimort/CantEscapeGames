
#include "CameraComponent.h"
#include "Graphics/Camera.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/EventManager.h"
#include "Events/Camera/CameraRegistrationEvent.h"


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
	EventManager::Get()->EnqueueEvent<CameraRegistrationEvent>(true, &m_camera, false);
}

CameraComponent::~CameraComponent()
{ }

void CameraComponent::Init(ResourceManager* resMgr)
{
	//TODO: Add a message to set this as the main camera if it has that property
	EventManager::Get()->EnqueueEvent<CameraRegistrationEvent>(true, &m_camera, true);
}

void CameraComponent::Begin()
{
}

const Camera& CameraComponent::GetCamera() const
{
	return m_camera;
}

void CameraComponent::SetCamera(const Camera& lhs)
{
	m_camera = lhs;
}