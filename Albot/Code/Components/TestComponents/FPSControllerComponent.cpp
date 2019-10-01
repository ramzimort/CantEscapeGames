
#include "FPSControllerComponent.h"
#include "Managers/EventManager.h"
#include "Events/Input/KeyDownEvent.h"
#include "GameObjects/GameObject.h"

unsigned const FPSControllerComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<FPSControllerComponent>("FPSController")
	.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
	.property("Inverted", &FPSControllerComponent::m_mouseInverted);
}

FPSControllerComponent::FPSControllerComponent(GameObject* go)
	: BaseComponent(go, static_type)
{
	EventManager::Get()->SubscribeEvent<KeyDownEvent>(static_cast<void*>(this), std::bind(
		&FPSControllerComponent::OnKeyDown, this, std::placeholders::_1));
}

FPSControllerComponent::~FPSControllerComponent()
{

}

void FPSControllerComponent::Init(ResourceManager* resMgr)
{

}

void FPSControllerComponent::Begin()
{

}

void FPSControllerComponent::OnKeyDown(const KeyDownEvent* event)
{

}