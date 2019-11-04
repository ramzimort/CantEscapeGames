#pragma once

#include "BaseComponent.h"
#include "../Events/Multicast.h"
#include "Helper/Hash.h"


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
	int GetTotalObjects();
	int GetTotalButtons();

	std::string GetStateAddress();

	void SetSliderValue(float val);
	void SetRenderEnable(bool val);
	bool GetRenderEnable();

	int GetChildButtonCount();
	int GetChildButtonLocation();

	float GetWidth();
	float GetHeight();

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


public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	
	// Total buttons UICamera will have
	int totalButtons;
	// Total objects UICamera will have
	int totalObjects;
	//Loading state of a the buttons
	StringId stateAddress;
	//Slider Value
	float sliderValue;
	//
	int childButtonCount;
	//
	int childButtonLocation;
	//  Inital position of the object
	Vector3 initialPos;
	// Final position of the object
	Vector3 finalPos;
	// Delta time for the animation
	float deltaTime;
	// Inital Roation Value
	float initailRotation;
	// Final Rotation Value
	float finalRotationValue;
	// Rate of Roation along Z axis
	float rotationRate;
	//
	float width;
	//
	float height;


	// ALL NEW STUFF
	int buttonIndex;
	bool m_clicked;
	bool m_enabled;
	std::string m_parentName;
	UIComponent* m_parent;
	std::vector<UIComponent*> m_children;

	// ALL NEW STUFF
	size_t m_index;
	bool m_clicked;
	bool m_enabled;
	std::string m_parentName;
	UIComponent* m_parent;
	std::vector<UIComponent*> m_children;


	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
