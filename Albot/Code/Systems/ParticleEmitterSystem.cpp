#include "ParticleEmitterSystem.h"
#include "Components/ParticleEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "GameObjects/GameObject.h"
#include "Graphics/AppRenderer.h"


unsigned const ParticleEmitterSystem::static_type = BaseSystem::numberOfTypes++;

ParticleEmitterSystem::ParticleEmitterSystem()
{
	Push_required_comp<ParticleEmitterComponent>();
	Push_required_comp<TransformComponent>();
}


ParticleEmitterSystem::~ParticleEmitterSystem()
{
}


void ParticleEmitterSystem::Update(float dt, BaseSystemCompNode *compNode)
{
}

void ParticleEmitterSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	ParticleEmitterCompNode* emit_comp_node  = static_cast<ParticleEmitterCompNode*>(compNode);
	TransformComponent* transformComp = emit_comp_node->m_transform;
	ParticleEmitterComponent* particleEmitterComp = emit_comp_node->m_particleEmitter;

	Vector3 gameobjPos = transformComp->GetPosition();

	if (!particleEmitterComp->m_firstTime)
	{
		std::swap(particleEmitterComp->m_pDrawStreamOutVB, particleEmitterComp->m_pStreamOutVB);
	}

	
	ParticleEmitterInstanceData emitterInstanceData = {};
	emitterInstanceData.m_firstTime = particleEmitterComp->m_firstTime;
	emitterInstanceData.m_pParticleEmitterUniformData.EmitterDirection = MathUtil::v3_to_v4(particleEmitterComp->m_emitterDirection, 0.f);
	emitterInstanceData.m_pParticleEmitterUniformData.EmitterPosition = MathUtil::v3_to_v4(gameobjPos + particleEmitterComp->m_emitterPosition, 1.f);
	emitterInstanceData.m_pParticleEmitterUniformData.EmitterMiscData.w = particleEmitterComp->m_infiniteEmitter ? 1.f : 0.f;
	emitterInstanceData.m_pParticleEmitterUniformData.EmitterMiscData.z = particleEmitterComp->m_emitterRate;
	emitterInstanceData.m_pParticleEmitterUniformData.EmitterMiscData.y = particleEmitterComp->m_emitterSpreadAngle;
	emitterInstanceData.m_pInitVB = particleEmitterComp->m_pInitVB;
	emitterInstanceData.m_pDrawStreamOutVB = particleEmitterComp->m_pDrawStreamOutVB;
	emitterInstanceData.m_pStreamOutVB = particleEmitterComp->m_pStreamOutVB;
	emitterInstanceData.m_pParticleTexture = particleEmitterComp->m_pParticleTexture;

	particleEmitterComp->m_firstTime = false;

	m_appRenderer->GetParticleRendering().RegisterParticleEmitterInstance(emitterInstanceData);
}


void ParticleEmitterSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_appRenderer = appRenderer;
}


void ParticleEmitterSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	ParticleEmitterComponent* particleEmitterComp = go->GetComponent<ParticleEmitterComponent>();

	BaseSystemCompNode *component_node = new ParticleEmitterCompNode(transform, particleEmitterComp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}