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
	const UIComponent* GetChild(size_t index) const;
	const size_t GetNumberOfChildren(size_t index) const;


	void IsTriggerd();
	void IsNotTriggered();

	void IsTouched();
	void IsNotTouched();
	int GetLocation();
	int GetTotalObjects();
	int GetTotalButtons();

	std::string GetStateAddress();

	void SetSliderValue(float val);

	int GetChildButtonCount();
	int GetChildButtonLocation();

	// Animation  Function
	Vector3 GetInitialPosition();
	Vector3 GetFinalPosition();
	Vector3 GetVelocity();
	float GetDeltaTime();
	float GetInitialRotation();
	float GetFinalRotation();
	float GetRotationRate();
	void InitialAnimationEnabled();
	void InitialAnimationDisabled();
	void FinalAnimationEnabled();
	void FinalAnimationDisabled();
	bool GetInitailAnimationState();
	bool GetFinalAnimationState();

public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	//Initial animation executed
	bool initialAnimationEnabled = false;
	
	bool finalAnimationEnabled = false;

	//Component count in state, (used when we want highlight a button when mouse cursor on it)
	int location;
	//if mouse on ui object
	bool isTouched;
	//if mouse on ui object
	bool isTriggerd;
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

	// ALL NEW STUFF
	size_t m_index;
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
