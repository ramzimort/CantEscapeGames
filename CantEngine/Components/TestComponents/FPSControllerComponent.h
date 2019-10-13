#pragma once

#include "Components/BaseComponent.h"

class KeyEvent;
class MouseEvent;
class GameObjectManager;

class FPSControllerComponent : public BaseComponent
{
public:
	FPSControllerComponent(GameObject* go);
	virtual ~FPSControllerComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer);
	virtual void Begin(GameObjectManager *goMgr);

	void OnKeyDown(const KeyEvent* event);
	void OnMouse(const MouseEvent* event);

public:
	//Unique class identifier
	static ComponentId const static_type;
	bool m_mouseInverted = false;
	bool m_forward = false;
	bool m_backward = false;
	bool m_left = false;
	bool m_right = false;

	Vector2 m_mousePosition;
	Vector2 m_mouseDeltaPosition;
	uint8_t m_leftClick;

private:

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};