#pragma once

#include "BaseComponent.h"
#include "../Events/Multicast.h"

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

	Multicast<void(int)> OnButtonClick;

	void IsTriggerd();
	void IsNotTriggered();

	void IsTouched();
	void IsNotTouched();
	int GetLocation();

public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	//State in which this component belongs
	int currentState;
	//After clicking the button it will go to next state
	int nextState;
	//Component count in state, (used when we want highlight a button when mouse cursor on it)
	int locationCount;
	//if mouse on ui object
	bool isTouched;
	//if mouse on ui object
	bool isTriggerd;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
