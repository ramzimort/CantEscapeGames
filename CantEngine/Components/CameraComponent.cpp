
#include "CameraComponent.h"
#include "Graphics/Camera.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"
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
		.property("isTransformIndependent", &CameraComponent::m_isTransformIndependent)
		.method("Init", &CameraComponent::Init);
}

CameraComponent::CameraComponent(GameObject *owner) : 
	BaseComponent(owner, CameraComponent::static_type),
	m_xViewportPos(0),
	m_yViewportPos(0),
	m_isTransformIndependent(false)
{
}

CameraComponent::~CameraComponent()
{ 
	EventManager::Get()->EnqueueEvent<CameraDestructionEvent>(true, &m_camera, m_camera.GetId());
}

void CameraComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	m_camera.InitFromComponent();
	EventManager::Get()->EnqueueEvent<CameraRegistrationEvent>(true, m_camera, m_xViewportPos, m_yViewportPos);
}

void CameraComponent::Begin(GameObjectManager *goMgr)
{
}

Camera& CameraComponent::GetCamera()
{
	return m_camera;
}