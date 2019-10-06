#pragma once
#include "BaseComponent.h"
#include "Graphics/Camera.h"

class Camera;

class CameraComponent :
	public BaseComponent
{
public:
	friend class CameraSystem;
public:
	CameraComponent(GameObject *owner);
	virtual ~CameraComponent();
	void Init(ResourceManager* resMgr);
	void Begin();
	Camera& GetCamera();
	void SetCamera(const Camera& lhs);
public:
	static ComponentId const static_type;

private:
	size_t m_xViewportPos;
	size_t m_yViewportPos;
	Camera m_camera;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;	
};

