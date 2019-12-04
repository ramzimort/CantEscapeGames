#pragma once

#include "BaseComponent.h"
#include "../Events/Multicast.h"
#include "Helper/Hash.h"
#include "Graphics/InstanceRenderData.h"


class UIComponent :	public BaseComponent
{
//Friend classes
public: 
	friend class UISystem;

public:
	UIComponent(GameObject *owner);
	virtual ~UIComponent();

	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	void Begin(GameObjectManager *goMgr) override;

	void RegisterChild(UIComponent* child);
	void UnregisterChild(UIComponent* child);

	const UIComponent* GetParent() const;
	const UIComponent* GetChild(int index) const;
	const int GetNumberOfChildren() const;
	const std::string GetParentName() const;



	int GetButtonIndex();

	void SetRenderEnable(bool val);
	bool GetRenderEnable();



	Vector3 GetUnTouchedScale();
	Vector3 GetUnTouchedScaleNormalized();
	void SetUnTouchedScale(const Vector3& val);

	Vector3 GetTouchedScale();
	Vector3 GetTouchedScaleNormalized();
	void SetTouchedScale(const Vector3& val);
	// Animation  Function
	Vector3 GetInitialPosition();
	Vector3 GetFinalPosition();
	Vector3 GetInitialPositionNormalized();
	Vector3 GetFinalPositionNormalized();
	void SetInitialPosition(const Vector3& val);
	void SetFinalPosition(const Vector3& val);
	Vector3 GetVelocity();
	float GetDeltaTime();
	float GetInitialRotation();
	float GetFinalRotation();
	float GetRotationRate();
	void SetTextScale(const Vector3& val);
	Vector3 GetTextScale();
	void SetText(const std::wstring& val);
	std::string GetText();

public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	

	//  Inital position of the object
	Vector3 initialPos; 
	// Final position of the object
	Vector3 finalPos;
	//
	Vector3 unTouchedScale;
	//
	Vector3 touchedScale;
	// Delta time for the animation
	float deltaTime;
	// Inital Roation Value
	float initailRotation;
	// Final Rotation Value
	float finalRotationValue;
	// Rate of Roation along Z axis
	float rotationRate;



	// ALL NEW STUFF
	int buttonIndex;
	bool m_clicked;
	bool m_enabled = true;
	std::string m_parentName;
	UIComponent* m_parent;
	std::vector<UIComponent*> m_children;

	std::string m_internalTextUI;
	std::wstring m_textUI;
	FontType m_textFontType;
	//position should be defined from 0 to 1 (origin is top left of windows coordinate)
	Vector2 m_textPosition;
	Vector3 m_textColor;
	Vector3 m_textScale;
	Vector3 m_textTouchedScale;
	float m_textRotation;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
