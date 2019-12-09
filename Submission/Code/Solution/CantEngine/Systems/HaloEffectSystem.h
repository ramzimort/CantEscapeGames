#pragma once
#include "BaseSystem.h"

class HaloEffectComponent;
class TransformComponent;
class AppRenderer;

struct HaloEffectCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	HaloEffectComponent* m_haloComp;
	//Ctor
	HaloEffectCompNode(TransformComponent *transform,
		HaloEffectComponent* haloComp) : m_transform(transform),
		m_haloComp(haloComp)
	{}
};


class HaloEffectSystem :
	public BaseSystem
{
public:
	HaloEffectSystem();
	virtual ~HaloEffectSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;

	void RegisterAppRenderer(AppRenderer* appRenderer);
public:
	//To compare when using templates
	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;
};

