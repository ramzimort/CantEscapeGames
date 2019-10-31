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

	void IsTriggerd();
	void IsNotTriggered();

	void IsTouched();
	void IsNotTouched();
	int GetLocation();
	int GetTotalObjects();
	int GetTotalButtons();

	void InitialAnimationEnabled();
	void InitialAnimationDisabled();

	void FinalAnimationEnabled();
	void FinalAnimationDisabled();

	bool GetInitailAnimationState();
	bool GetFinalAnimationState();

	std::string GetStateAddress();

	void SetSliderValue(float val);

	int GetChildButtonCount();
	int GetChildButtonLocation();

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


	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
