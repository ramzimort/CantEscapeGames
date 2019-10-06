#pragma once
#include "Shaders/Shading.h"
#include "BaseComponent.h"
#include "Graphics/InstanceRenderData.h"

class Buffer;


class ParticleEmitterComponent :
	public BaseComponent
{
public:
	friend class ParticleEmitterSystem;
public:
	ParticleEmitterComponent(GameObject *owner);
	virtual ~ParticleEmitterComponent();

	void Init(ResourceManager* resourceManager);
	void Begin() {}
public:
	static ComponentId const static_type;
private:

	ParticleEmitterInstanceData m_particleEmitterInstanceData;

	std::string m_particleTextureDir;
	Texture* m_pParticleTexture;

	//local to the game object current position
	Vector3 m_emitterPosition;

	Vector3 m_emitterDirection;
	float m_emitterSpreadAngle;
	float m_emitterRate;
	bool m_infiniteEmitter;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

