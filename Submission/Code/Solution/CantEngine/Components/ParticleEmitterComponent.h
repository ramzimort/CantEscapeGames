#pragma once
#include "Shaders/Shading.h"
#include "BaseComponent.h"
#include "Graphics/InstanceRenderData.h"
#include "Helper/Hash.h"

class Buffer;


enum ParticleEmitterType
{
	INFINITE_LIFETIME,
	FINITE_LIFETIME,
	EMIT_ONCE
};


class ParticleEmitterComponent :
	public BaseComponent
{
public:
	friend class ParticleEmitterSystem;
public:
	ParticleEmitterComponent(GameObject *owner);
	virtual ~ParticleEmitterComponent();

	void Init(ResourceManager* resourceManager, DXRenderer* dxrenderer);
	void Begin(GameObjectManager *goMgr) override{}

	void Emit();

	bool FinishedEmitting() const;

	void SetParticleSize(const Vector2& particleSize);
	void SetParticleEmitter(ParticleEmitterType particleEmitterType);
	void SetParticleEmitterMainDirection(const Vector3& emitterDirection);
	
	void SetEmitterDirection(const Vector3& dir);
	void SetEmitterSpreadAngleYaw(float degree);
	void SetEmitterSpreadAnglePitch(float degree);
	//in seconds
	void SetEmitterRate(float rate);
	void SetEmitterLifetime(float lifetime);

	void SetEmitParticlesCount(uint32_t emitCount);
public:
	static ComponentId const static_type;
private:
	float m_localTime;

	Buffer* m_pInitVB;
	Buffer* m_pStreamOutVB;
	Buffer* m_pDrawStreamOutVB;
	bool m_firstTime;

	StringId m_particleTextureDir;
	Texture* m_pParticleTexture;

	//local to the game object current position
	Vector3 m_emitterPosition;

	Vector3 m_emitterDirection;
	float m_emitterSpreadAngleYaw;
	float m_emitterSpreadAnglePitch;
	float m_emitterRate;
	float m_emitterLifetime;
	
	//How many particles being emitted when the emitter is emitted
	uint32_t m_emitParticleCount;
	
	Vector2 m_particleSize;
	ParticleEmitterType m_particleEmitterType;

	bool m_isEmitting;


	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

