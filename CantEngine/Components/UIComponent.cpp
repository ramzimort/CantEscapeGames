#include "UIComponent.h"

unsigned const UIComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<UIComponent>("UIComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &UIComponent::Init);
}

UIComponent::UIComponent(GameObject *owner) :BaseComponent(owner, static_type)
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