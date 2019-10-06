
#include "CameraComponent.h"
#include "Graphics/Camera.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/EventManager.h"
#include "Events/Camera/CameraEvents.h"


unsigned const CameraComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<CameraComponent>("CameraComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("Camera", &CameraComponent::m_camera)
		.property("xViewportPos", &CameraComponent::m_xViewportPos)
		.property("yViewportPos", &CameraComponent::m_yViewportPos)
		.method("Init", &CameraComponent::Init);
}

CameraComponent::CameraComponent(GameObject *owner) : 
	BaseComponent(owner, CameraComponent::static_type),
	m_xViewportPos(0),
	m_yViewportPos(0)
{
}

CameraComponent::~CameraComponent()
{ 
	EventManager::Get()->EnqueueEvent<CameraDestructionEvent>(true, m_camera.GetId());
}

void CameraComponent::Init(ResourceManager* resMgr)
{
	EventManager::Get()->EnqueueEvent<CameraRegistrationEvent>(true, m_camera, m_xViewportPos, m_yViewportPos);
}

void CameraComponent::Begin()
{
}

Camera& CameraComponent::GetCamera()
{
	return m_camera;
}

void CameraComponent::SetCamera(const Camera& lhs)
{
	m_camera = lhs;
}