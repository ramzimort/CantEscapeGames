#pragma once

#include "Components/BaseComponent.h"

class KeyEvent;
class MouseEvent;

class FPSControllerComponent : public BaseComponent
{
public:
	FPSControllerComponent(GameObject* go);
	virtual ~FPSControllerComponent();

	virtual void Init(ResourceManager* resMgr);
	virtual void Begin();

	void OnKeyDown(const KeyEvent* event);
	void OnMouse(const MouseEvent* event);

public:
	//Unique class identifier
	static ComponentId const static_type;
	bool m_mouseInverted;
	bool m_forward;
	bool m_backward;
	bool m_left;
	bool m_right;

	Vector2 m_mousePosition;
	Vector2 m_mouseDeltaPosition;
	uint8_t m_leftClick;

private:

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};