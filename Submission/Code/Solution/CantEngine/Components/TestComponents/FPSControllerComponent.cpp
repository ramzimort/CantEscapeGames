
#include "FPSControllerComponent.h"
#include "Managers/EventManager.h"
#include "Events/Input/KeyEvent.h"
#include "Events/Input/MouseEvent.h"
#include "GameObjects/GameObject.h"
#include "Managers/GameObjectManager.h"

unsigned const FPSControllerComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<FPSControllerComponent>("FPSController")
	.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr);
}

FPSControllerComponent::FPSControllerComponent(GameObject* go)
	: BaseComponent(go, static_type)
{
	EventManager::Get()->SubscribeEvent<KeyEvent>(this, 
		std::bind(&FPSControllerComponent::OnKeyDown, this, std::placeholders::_1));

	EventManager::Get()->SubscribeEvent<MouseEvent>(this,
		std::bind(&FPSControllerComponent::OnMouse, this, std::placeholders::_1));
}

FPSControllerComponent::~FPSControllerComponent()
{
	EventManager::Get()->UnsubscribeEvent<KeyEvent>(this);
	EventManager::Get()->UnsubscribeEvent<MouseEvent>(this);
}

void FPSControllerComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{

}

void FPSControllerComponent::Begin(GameObjectManager *goMgr)
{

}

void FPSControllerComponent::OnKeyDown(const KeyEvent* event)
{
	switch (event->m_scancode)
	{
	case SDL_SCANCODE_W:
		m_forward = event->m_press; break;
	case SDL_SCANCODE_A:
		m_left = event->m_press; break;
	case SDL_SCANCODE_D:
		m_right = event->m_press; break;
	case SDL_SCANCODE_S:
		m_backward = event->m_press; break;
	}
}

void FPSControllerComponent::OnMouse(const MouseEvent* event)
{
	m_mousePosition = event->m_position;
	m_mouseDeltaPosition = event->m_deltaPosition;
	m_leftClick = event->m_state & SDL_BUTTON(SDL_BUTTON_LEFT);
}