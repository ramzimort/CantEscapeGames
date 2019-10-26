#pragma once

#include "BaseComponent.h"

class AffineAnimationComponent : public BaseComponent
{
	//Friend classes
public:
	friend class UISystem;

public:
	AffineAnimationComponent(GameObject *owner);
	virtual ~AffineAnimationComponent();

	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	void Begin(GameObjectManager *goMgr) override;

	Vector3 GetInitialPosition();
	Vector3 GetFinalPosition();
	Vector3 GetVelocity();
	float GetDeltaTime();

public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	//  Inital position of the object
	Vector3 initialPos;
	// Final position of the object
	Vector3 finalPos;
	// Delta time for the animation
	float deltaTime;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};