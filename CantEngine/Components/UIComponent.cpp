#include "UIComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

unsigned const UIComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<UIComponent>("UIComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("CurrentState", &UIComponent::currentState)
		.property("NextState", &UIComponent::nextState)
		.property("LocationCount", &UIComponent::locationCount)
		.method("Init", &UIComponent::Init);
}

UIComponent::UIComponent(GameObject *owner) :BaseComponent(owner, static_type), isTriggerd(false), isTouched(false)
{

}
UIComponent::~UIComponent()
{
}

void UIComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
}

void UIComponent::Begin(GameObjectManager *goMgr)
{

}

void UIComponent::IsTriggerd()
{
	isTriggerd = true;
}
void UIComponent::IsNotTriggered()
{
	isTriggerd = false;
}

void UIComponent::IsTouched()
{
	isTouched = true;
}
void UIComponent::IsNotTouched()
{
	isTouched = false;
}
int UIComponent::GetLocation()
{
	return locationCount;
}
