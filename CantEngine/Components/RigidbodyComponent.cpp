/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RigidbodyComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/GameObjectManager.h"
#include "Graphics/D3D11_Renderer.h"

unsigned const RigidbodyComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<RigidbodyComponent>("RigidBodyComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &RigidbodyComponent::Init)
		.property("IsEffectedByGravity", &RigidbodyComponent::m_isEffectedByGravity)
		.property("Velocity", &RigidbodyComponent::m_velocity)
		.property("Mass", &RigidbodyComponent::m_mass)
		.property("IsAsleep", &RigidbodyComponent::m_isAsleep)
		.property("CollisionMask", &RigidbodyComponent::m_collisionMask)
	;
	rttr::registration::enumeration<CollisionTable::CollisionMask>("CollisionMask")(
		#define COLLISION_MASK(ENTRY) rttr::value(#ENTRY, CollisionTable::CollisionMask::ENTRY),
		#include "Physics/CollisionMaskTypes.def"
		#undef COLLISION_MASK
		rttr::value("NUM", CollisionTable::CollisionMask::NUM));
}

RigidbodyComponent::RigidbodyComponent(GameObject *owner) :
	BaseComponent(owner, RigidbodyComponent::static_type)
{
}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	m_inertiaTensor = m_inertiaTensorInverse = Matrix::Identity; // this works for cubes
	m_inverseMass = 1.0f / m_mass;
}

void RigidbodyComponent::Begin(GameObjectManager *goMgr)
{
}

const Vector3& RigidbodyComponent::GetPosition() const
{
	return m_position;
}

const Aabb& RigidbodyComponent::GetAabb() const
{
	return m_aabb;
}

const Vector3& RigidbodyComponent::GetVelocity() const
{
	return m_velocity;
}

void RigidbodyComponent::SetVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

const Vector3& RigidbodyComponent::GetAngularVelocity() const
{
	return m_angularVelocity;
}

void RigidbodyComponent::SetAngularVelocity(const Vector3& angularVelocity)
{
	m_angularVelocity = angularVelocity;
}

float RigidbodyComponent::GetMass() const
{
	return m_mass;
}

void RigidbodyComponent::SetMass(float mass)
{
	m_mass = mass;
	m_inverseMass = 1.0f / mass;
}

CollisionTable::CollisionMask RigidbodyComponent::GetCollisionMask()
{
	return m_collisionMask;
}

bool RigidbodyComponent::IsAsleep() const 
{
	return m_isAsleep;
}

void RigidbodyComponent::InitInertiaTensor(float x, float y, float z)
{
	m_inverseMass = 1.0f / m_mass;
	if (m_inverseMass < PhysicsUtils::Consts::minMass)
	{
		m_inverseMass = 0.0f;
		m_inertiaTensor = m_inertiaTensorInverse = Matrix() * 0.0f;
		m_inertiaTensor.m[3][3] = m_inertiaTensorInverse.m[3][3] = 1.0f;
		return;
	}

	float massCoef = m_mass / 12.0f;
	m_inertiaTensor.m[0][0] = (y * y + z * z);
	m_inertiaTensor.m[1][1] = (x * x + z * z);
	m_inertiaTensor.m[2][2] = (x * x + y * y);

	m_inertiaTensor.m[0][1] = m_inertiaTensor.m[1][0] = m_inertiaTensor.m[0][2] = 
		m_inertiaTensor.m[2][0] = m_inertiaTensor.m[1][2] = m_inertiaTensor.m[2][1] = 
		m_inertiaTensor.m[0][3] = m_inertiaTensor.m[1][3] = m_inertiaTensor.m[2][3] =
		m_inertiaTensor.m[3][0] = m_inertiaTensor.m[3][1] = m_inertiaTensor.m[3][2] = 0.0f;

	m_inertiaTensor *= massCoef; 
	m_inertiaTensor.m[3][3] = 1.0f;
	m_inertiaTensorInverse = m_inertiaTensor.Invert();
}
