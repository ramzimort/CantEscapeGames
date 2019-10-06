#include "ParticleEmitterComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"

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
	m_infiniteEmitter(false)
{
}


ParticleEmitterComponent::~ParticleEmitterComponent()
{
}


void ParticleEmitterComponent::Init(ResourceManager* resourceManager)
{
	m_emitterDirection.Normalize();
}