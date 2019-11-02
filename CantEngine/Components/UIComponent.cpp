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
		.property("InitialPosition", &UIComponent::initialPos)
		.property("FinalPosition", &UIComponent::finalPos)
		.property("DeltaTime", &UIComponent::deltaTime)
		.property("InitialRotation", &UIComponent::initailRotation)
		.property("FinalRotation", &UIComponent::finalRotationValue)
		.property("RotationRate", &UIComponent::rotationRate)
		.method("Init", &UIComponent::Init);
}

UIComponent::UIComponent(GameObject* owner) : 
	BaseComponent(owner, static_type), 
	isTriggerd(false), 
	isTouched(false), 
	m_parent(nullptr), 
	m_children(std::vector<UIComponent*>(0))
{
}

UIComponent::~UIComponent()
{
	if (m_parent != nullptr)
	{
		UnregisterChild(this);
	}
}

void UIComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
}

void UIComponent::Begin(GameObjectManager *goMgr)
{
	GameObject* go = goMgr->FindGameObject(m_parentName);
	if (go)
	{
		m_parent = go->GetComponent<UIComponent>();
		m_parent->RegisterChild(this);
	}
}

void UIComponent::RegisterChild(UIComponent* child)
{
	m_children.push_back(child);
}

void UIComponent::UnregisterChild(UIComponent* child)
{
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*it == child)
		{
			m_children.erase(it);
			break;
		}
	}
}

const UIComponent* UIComponent::GetParent() const
{
	return m_parent;
}

const UIComponent* UIComponent::GetChild(size_t index) const
{
	if (index < m_children.size())
	{
		return m_children[index];
	}
	return nullptr;
}

const size_t UIComponent::GetNumberOfChildren(size_t index) const
{
	return m_children.size();
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
Vector3 UIComponent::GetInitialPosition()
{
	return initialPos;
}

Vector3 UIComponent::GetFinalPosition()
{
	return finalPos;
}
Vector3 UIComponent::GetVelocity()
{
	Vector3 result = finalPos - initialPos;
	result.Normalize();
	return result;
}
float UIComponent::GetDeltaTime()
{
	return deltaTime;
}

float UIComponent::GetInitialRotation()
{
	return initailRotation;
}

float UIComponent::GetFinalRotation()
{
	return finalRotationValue;
}

float UIComponent::GetRotationRate()
{
	return rotationRate;
}




