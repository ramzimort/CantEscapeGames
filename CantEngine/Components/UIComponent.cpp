#include "UIComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"
#include "Helper/Hash.h"

unsigned const UIComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::enumeration<FontType>("FontType")(
		rttr::value("Courier_New", FontType::COURIER_NEW),
		rttr::value("Courier_New_Bold", FontType::COURIER_NEW_BOLD)
		);

	rttr::registration::class_<UIComponent>("UIComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("ParentName", &UIComponent::m_parentName)
		.property("ButtonIndex", &UIComponent::buttonIndex)
		.property("InitialPosition", &UIComponent::initialPos)
		.property("FinalPosition", &UIComponent::finalPos)
		.property("DeltaTime", &UIComponent::deltaTime)
		.property("InitialRotation", &UIComponent::initailRotation)
		.property("FinalRotation", &UIComponent::finalRotationValue)
		.property("RotationRate", &UIComponent::rotationRate)
		.property("Enabled", &UIComponent::m_enabled)
		.property("Text", &UIComponent::m_internalTextUI)
		.property("TextFont", &UIComponent::m_textFontType)
		.property("TextPosition", &UIComponent::m_textPosition)
		.property("TextRotation", &UIComponent::m_textRotation)
		.property("TextScale", &UIComponent::m_textScale)
		.property("TextColor", &UIComponent::m_textColor)
		.method("Init", &UIComponent::Init);
}

UIComponent::UIComponent(GameObject* owner) : 
	BaseComponent(owner, static_type), 
	m_parent(nullptr), m_enabled(true),
	m_children(std::vector<UIComponent*>(0)),
	m_textUI(L""),
	m_textFontType(FontType::COURIER_NEW),
	m_textPosition(0.f),
	m_textRotation(0.f),
	m_textScale(1.f),
	m_textColor(1.f, 1.f, 1.f)
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
	m_textUI = std::wstring(m_internalTextUI.begin(), m_internalTextUI.end());
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

const UIComponent* UIComponent::GetChild(int index) const
{
	if (index < m_children.size())
	{
		return m_children[index];
	}
	return nullptr;
}

const int UIComponent::GetNumberOfChildren() const
{
	return int(m_children.size());
}

const std::string UIComponent::GetParentName() const
{
	return m_parentName;
}


int UIComponent::GetButtonIndex()
{
	return buttonIndex;
}


void UIComponent::SetRenderEnable(bool val)
{
	m_enabled = val;
}

bool UIComponent::GetRenderEnable()
{
	return m_enabled;
}

Vector3 UIComponent::GetUnTouchedScale()
{
	return unTouchedScale;
}

void UIComponent::SetUnTouchedScale(const Vector3 & val)
{
	unTouchedScale = val;
}

Vector3 UIComponent::GetTouchedScale()
{
	return touchedScale;
}

void UIComponent::SetTouchedScale(const Vector3 & val)
{
	touchedScale = val;
}


void UIComponent::SetTextScale(const Vector3 & val)
{
	m_textScale = val;
}

Vector3 UIComponent::GetTextScale()
{
	return m_textScale;
}
Vector3 UIComponent::GetInitialPosition()
{
	return initialPos;
}

Vector3 UIComponent::GetFinalPosition()
{
	return finalPos;
}
Vector3 UIComponent::GetInitialPositionNormalized()
{
	Vector3 result = initialPos;
	result.Normalize();
	return result;
}
Vector3 UIComponent::GetFinalPositionNormalized()
{
	Vector3 result = finalPos;
	result.Normalize();
	return result;
}
Vector3 UIComponent::GetUnTouchedScaleNormalized()
{
	Vector3 result = unTouchedScale;
	result.Normalize();
	return result;
}
Vector3 UIComponent::GetTouchedScaleNormalized()
{
	Vector3 result = touchedScale;
	result.Normalize();
	return result;
}
void UIComponent::SetInitialPosition(const Vector3& val)
{
	initialPos = val;
}
void UIComponent::SetFinalPosition(const Vector3& val)
{
	finalPos = val;
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

void UIComponent::SetText(const std::string& val)
{
	m_textUI = std::wstring(val.begin(), val.end());
}
std::string UIComponent::GetText()
{
	return m_internalTextUI;
}



