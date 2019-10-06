#include "ParticleEmitterSystem.h"
#include "Components/ParticleEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "GameObjects/GameObject.h"


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