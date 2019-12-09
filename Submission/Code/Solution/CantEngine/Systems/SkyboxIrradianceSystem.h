#pragma once
#include "BaseSystem.h"


class TransformComponent;
class SkyboxIrradianceComponent;
class AppRenderer;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct SkyboxIrradianceCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	SkyboxIrradianceComponent *m_skyboxIrradianceComp;

	//Ctor
	SkyboxIrradianceCompNode(TransformComponent *transform,
		SkyboxIrradianceComponent *skyboxIrradianceComp) : m_transform(transform),
		m_skyboxIrradianceComp(skyboxIrradianceComp)
	{}
};



class SkyboxIrradianceSystem :
	public BaseSystem
{
public:
	SkyboxIrradianceSystem();
	virtual ~SkyboxIrradianceSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;
	void RegisterAppRenderer(AppRenderer* appRenderer);
public:
	//To compare when using templates
	static unsigned int const static_type;
private:
	AppRenderer* m_appRenderer;
};

