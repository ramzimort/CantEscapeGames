
#include "TriggerComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/GameObjectManager.h"
#include "Graphics/D3D11_Renderer.h"

unsigned const TriggerComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<TriggerComponent>("TriggerComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &TriggerComponent::Init)
		.property("Scale", &TriggerComponent::m_scale)
		.property("Offset", &TriggerComponent::m_offset)
		.property("CollisionMask", &TriggerComponent::m_collisionMask)
	;
}

TriggerComponent::TriggerComponent(GameObject* owner) 
	: BaseComponent(owner, TriggerComponent::static_type)
	, m_dynamicAabbTreeKey(0)
{
}

TriggerComponent::~TriggerComponent()
{
}

void TriggerComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	UpdateAabb();
}

void TriggerComponent::Begin(GameObjectManager* goMgr)
{
}

const Aabb& TriggerComponent::GetAabb() const
{
	return m_aabb;
}

const Vector3& TriggerComponent::GetScale()
{
	return m_scale;
}

void TriggerComponent::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

const Vector3& TriggerComponent::GetOffset()
{
	return m_offset;
}

void TriggerComponent::SetOffset(const Vector3& offset)
{
	m_offset = offset;
}


CollisionTable::CollisionMask TriggerComponent::GetCollisionMask()
{
	return m_collisionMask;
}

void TriggerComponent::UpdateAabb()
{
	m_aabb.m_Max = m_scale * 0.5f;
	m_aabb.m_Min = -m_aabb.m_Max;
}
