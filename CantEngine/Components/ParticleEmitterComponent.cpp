#include "ParticleEmitterComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/DXResourceLoader.h"

#define MAX_PARTICLE_COUNT 5000

unsigned const ParticleEmitterComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<ParticleEmitterComponent>("ParticleEmitterComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("Emitter_Position", &ParticleEmitterComponent::m_emitterPosition)
		.property("Emitter_Direction", &ParticleEmitterComponent::m_emitterDirection)
		.property("Emitter_Spread_Angle_Yaw", &ParticleEmitterComponent::m_emitterSpreadAngleYaw)
		.property("Emitter_Spread_Angle_Pitch", &ParticleEmitterComponent::m_emitterSpreadAnglePitch)
		.property("Emitter_Rate", &ParticleEmitterComponent::m_emitterRate)
		.property("Emitter_Lifetime", &ParticleEmitterComponent::m_emitterLifetime)
		.property("Particle_Emit_Count", &ParticleEmitterComponent::m_emitParticleCount)
		.property("Particle_Texture_Dir", &ParticleEmitterComponent::m_particleTextureDir)
		.property("Particle_Emitter_Type", &ParticleEmitterComponent::m_particleEmitterType)
		.method("Init", &ParticleEmitterComponent::Init);

	rttr::registration::enumeration<ParticleEmitterType>("ParticleEmitterType")(
		rttr::value("Infinite_Lifetime", ParticleEmitterType::INFINITE_LIFETIME),
		rttr::value("Finite_Lifetime", ParticleEmitterType::FINITE_LIFETIME),
		rttr::value("Emit_Once", ParticleEmitterType::EMIT_ONCE));
}



ParticleEmitterComponent::ParticleEmitterComponent(GameObject *owner)
	:BaseComponent(owner, static_type),
	m_emitterDirection(0.f, 1.f, 0.f),
	m_emitterPosition(0.f),
	m_emitterRate(0.005f),
	m_emitterSpreadAngleYaw(360.f),
	m_emitterSpreadAnglePitch(90.f),
	m_firstTime(true),
	m_emitterLifetime(5.f),
	m_localTime(0.f),
	m_emitParticleCount(50),
	m_particleEmitterType(ParticleEmitterType::INFINITE_LIFETIME),
	m_isEmitting(true)
{
}


ParticleEmitterComponent::~ParticleEmitterComponent()
{
	SafeReleaseDelete(m_pInitVB);
	SafeReleaseDelete(m_pStreamOutVB);
	SafeReleaseDelete(m_pDrawStreamOutVB);
}


void ParticleEmitterComponent::Init(ResourceManager* resourceManager, DXRenderer* dxrenderer)
{
	m_emitterDirection.Normalize();

	Particle initParticle = {};
	initParticle.m_lifeTime = 0.f;
	initParticle.m_position = Vector3(0.f);
	initParticle.m_size = Vector2(1.f, 1.f);
	initParticle.m_particleType = PARTICLE_TYPE_EMITTER;

	BufferLoadDesc streamout_vb_desc = {};
	streamout_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
	streamout_vb_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	streamout_vb_desc.m_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	streamout_vb_desc.m_desc.m_vertexStride = sizeof(Particle);
	streamout_vb_desc.m_rawData = &initParticle;
	streamout_vb_desc.m_size = streamout_vb_desc.m_desc.m_vertexStride * 1;

	m_pInitVB = DXResourceLoader::Create_Buffer(dxrenderer, streamout_vb_desc);

	streamout_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER | Bind_Flags::BIND_STREAM_OUTPUT;
	streamout_vb_desc.m_size = streamout_vb_desc.m_desc.m_vertexStride * MAX_PARTICLE_COUNT;
	streamout_vb_desc.m_rawData = nullptr;

	m_pStreamOutVB = DXResourceLoader::Create_Buffer(dxrenderer, streamout_vb_desc);
	m_pDrawStreamOutVB = DXResourceLoader::Create_Buffer(dxrenderer, streamout_vb_desc);

	m_pParticleTexture = resourceManager->GetTexture(m_particleTextureDir);
}

void ParticleEmitterComponent::Emit()
{
	m_localTime = 0.f;
	m_firstTime = true;
	m_isEmitting = true;
}


void ParticleEmitterComponent::SetEmitterDirection(const Vector3& dir)
{
	Vector3 localDir = dir;
	localDir.Normalize();
	m_emitterDirection = localDir;
}

void ParticleEmitterComponent::SetParticleEmitter(ParticleEmitterType particleEmitterType)
{
	m_particleEmitterType = particleEmitterType;
}

void ParticleEmitterComponent::SetEmitterSpreadAngleYaw(float degree)
{
	m_emitterSpreadAngleYaw = degree;
}

void ParticleEmitterComponent::SetEmitterSpreadAnglePitch(float degree)
{
	m_emitterSpreadAnglePitch = degree;
}

//in seconds
void ParticleEmitterComponent::SetEmitterRate(float rate)
{
	m_emitterRate = rate;
}

void ParticleEmitterComponent::SetEmitterLifetime(float lifetime)
{
	m_emitterLifetime = lifetime;
}

void ParticleEmitterComponent::SetEmitParticlesCount(uint32_t emitCount)
{
	m_emitParticleCount = std::min(emitCount, MAX_PARTICLE_EMIT_ONCE);
}


bool ParticleEmitterComponent::FinishedEmitting() const
{
	if (m_particleEmitterType == ParticleEmitterType::EMIT_ONCE)
	{
		//return !m_firstTime;
		return m_localTime > m_emitterLifetime;
	}
	else if (m_particleEmitterType == ParticleEmitterType::FINITE_LIFETIME)
	{
		return m_localTime > m_emitterLifetime;
	}
	return false;
}