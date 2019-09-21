#pragma once
#include "BaseComponent.h"

class Camera;

class CameraComponent :
	public BaseComponent
{
public:
	CameraComponent(GameObject *owner, ComponentId type);
	virtual ~CameraComponent();


	Camera* GetCamera() const;
public:
	static ComponentId const static_type;



private:
	Camera* m_camera;
	//Unique class identifier
	
};

