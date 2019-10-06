#pragma once
#include "BaseComponent.h"
#include "Graphics/Camera.h"

class Camera;

class CameraComponent :
	public BaseComponent
{
public:
	friend class FPSCameraSystem;
public:
	CameraComponent(GameObject *owner);
	virtual ~CameraComponent();

	
	void Init(ResourceManager* resMgr);
	void Begin();
	const Camera& GetCamera() const;
	void SetCamera(const Camera& lhs);

public:
	static ComponentId const static_type;

private:
	Camera m_camera;

	//Unique class identifier

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;	
};

