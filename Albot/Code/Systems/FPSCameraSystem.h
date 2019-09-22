#pragma once
#include "BaseSystem.h"

class Transform;
class CameraComponent;

struct FPSCameraCompNode : BaseSystemCompNode
{
	Transform *m_transform;
	CameraComponent* m_camera_comp;

	//Ctor
	FPSCameraCompNode(Transform *transform,
		CameraComponent* camera_comp) : m_transform(transform),
		m_camera_comp(camera_comp)
	{}
};


class FPSCameraSystem :
	public BaseSystem
{
public:
	FPSCameraSystem();
	virtual ~FPSCameraSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;
public:
	//To compare when using templates
	static unsigned int const static_type;
};

