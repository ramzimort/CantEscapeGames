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
		.property("Emitter_Spread_Angle", &ParticleEmitterComponent::m_emitterSpreadAngle)
		.property("Emitter_Rate", &ParticleEmitterComponent::m_emitterRate)
		.property("Particle_Texture_Dir", &ParticleEmitterComponent::m_particleTextureDir)
		.property("Infinite_Emitter_Flag", &ParticleEmitterComponent::m_infiniteEmitter)
		.method("Init", &ParticleEmitterComponent::Init);

}



ParticleEmitterComponent::ParticleEmitterComponent(GameObject *owner)
	:BaseComponent(owner, static_type),
	m_emitterDirection(0.f),
	m_emitterPosition(0.f),
	m_emitterRate(0.005f),
	m_emitterSpreadAngle(30.f),
	m_infiniteEmitter(false),
	m_firstTime(true)
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

	m_pParticleTexture = resourceManager->GetTexture(StringId(m_particleTextureDir));
}