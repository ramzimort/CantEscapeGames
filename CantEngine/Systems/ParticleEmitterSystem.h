#pragma once
#include "BaseSystem.h"

class TransformComponent;
class ParticleEmitterComponent;
class AppRenderer;
class GameObject;


struct ParticleEmitterCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	ParticleEmitterComponent* m_particleEmitter;
	//Ctor
	ParticleEmitterCompNode(TransformComponent *transform,
		ParticleEmitterComponent* particleEmittercomp) : m_transform(transform),
		m_particleEmitter(particleEmittercomp)
	{}
};

class ParticleEmitterSystem :
	public BaseSystem
{
public:
	ParticleEmitterSystem();
	~ParticleEmitterSystem();

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

