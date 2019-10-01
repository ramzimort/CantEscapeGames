#pragma once

#include "Components/BaseComponent.h"

class KeyDownEvent;

class FPSControllerComponent : public BaseComponent
{
public:
	FPSControllerComponent(GameObject* go);
	virtual ~FPSControllerComponent();

	virtual void Init(ResourceManager* resMgr);
	virtual void Begin();

	void OnKeyDown(const KeyDownEvent* event);

public:
	//Unique class identifier
	static ComponentId const static_type;
	bool m_mouseInverted;
	bool m_forward;
	bool m_backward;
	bool m_left;
	bool m_right;
	bool m_mouse_left_click;
	bool m_mouse_right_click;

private:

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};