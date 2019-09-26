#pragma once
#include "BaseComponent.h"

class Camera;

class CameraComponent :
	public BaseComponent
{
public:
	friend class FPSCameraSystem;
public:
	CameraComponent(GameObject *owner);
	virtual ~CameraComponent();

	
	void Init();
	void Begin();
	Camera* GetCamera() const;
	void SetCamera(Camera* camera);
public:
	static ComponentId const static_type;

private:
	Camera* m_camera;
	//Unique class identifier

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;	
};

