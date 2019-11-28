#pragma once
#include "BaseComponent.h"
#include "Graphics/Camera.h"

class Camera;
class GameObjectManager;

class CameraComponent :
	public BaseComponent
{
public:
	friend class CameraSystem;
public:
	CameraComponent(GameObject *owner);
	virtual ~CameraComponent();

	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer);
	void Begin(GameObjectManager *goMgr);
	Camera& GetCamera();
public:
	static ComponentId const static_type;

private:
	size_t m_xViewportPos;
	size_t m_yViewportPos;
	Camera m_camera;

	//This flag determines wether the camera component positions 
	//its camera WRT the transform component every frame
	bool m_isTransformIndependent;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;	
};

