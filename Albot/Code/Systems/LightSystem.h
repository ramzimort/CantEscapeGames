#pragma once
#include "BaseSystem.h"

class TransformComponent;
class LightComponent;

struct LightCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	LightComponent* m_light_comp;
	//Ctor
	LightCompNode(TransformComponent *transform,
		LightComponent* light_comp) : m_transform(transform),
		m_light_comp(light_comp)
	{}
};

class LightSystem :
	public BaseSystem
{
public:
	LightSystem();
	virtual ~LightSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;
public:
	//To compare when using templates
	static unsigned int const static_type;
};

