#include "UIComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"
#include "Helper/Hash.h"

unsigned const UIComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<UIComponent>("UIComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("State", &UIComponent::stateAddress)
		.property("TotalObjects", &UIComponent::totalObjects)
		.property("TotalButtons", &UIComponent::totalButtons)
		.property("Location", &UIComponent::location)
		.property("SliderValue", &UIComponent::sliderValue)
		.property("ChildButtonCount", &UIComponent::childButtonCount)
		.property("ChildButtonLocation", &UIComponent::childButtonLocation)
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
	return location;
}

std::string UIComponent::GetStateAddress()
{
	return stateAddress.getName();
}

void UIComponent::SetSliderValue(float val)
{
	sliderValue = val;
}

int UIComponent::GetChildButtonCount()
{
	return childButtonCount;
}

int UIComponent::GetChildButtonLocation()
{
	return childButtonLocation;
}

int UIComponent::GetTotalObjects()
{
	return totalObjects;
}

int UIComponent::GetTotalButtons()
{
	return totalButtons;
}

void UIComponent::InitialAnimationEnabled()
{
	initialAnimationEnabled = true;
}

void UIComponent::InitialAnimationDisabled()
{
	initialAnimationEnabled = false;
}

void UIComponent::FinalAnimationEnabled()
{
	finalAnimationEnabled = true;
}

void UIComponent::FinalAnimationDisabled()
{
	finalAnimationEnabled = false;
}

bool UIComponent::GetInitailAnimationState()
{
	return initialAnimationEnabled;
}

bool UIComponent::GetFinalAnimationState()
{
	return finalAnimationEnabled;
}




