#pragma once
#include "BaseSystem.h"

class TransformComponent;
class CameraComponent;

struct CameraCompNode : BaseSystemCompNode
{
	TransformComponent *m_pTransformComponent;
	CameraComponent* m_pCameraComponent;

	CameraCompNode(TransformComponent *transformComp, CameraComponent* cameraComp) 
		: m_pTransformComponent(transformComp),	m_pCameraComponent(cameraComp) { }
};

class CameraSystem :
	public BaseSystem
{
public:
	CameraSystem();
	virtual ~CameraSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;
public:
	//To compare when using templates
	static unsigned int const static_type;
};

