/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RigidbodyComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"

unsigned const RigidbodyComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<RigidbodyComponent>("RigidBodyComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &RigidbodyComponent::Init)
		.property("IsEffectedByGravity", &RigidbodyComponent::m_isEffectedByGravity)
		.property("Velocity", &RigidbodyComponent::m_velocity)
	;
}

RigidbodyComponent::RigidbodyComponent(GameObject *owner) :
	BaseComponent(owner, RigidbodyComponent::static_type)
{
}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Init(ResourceManager* resMgr)
{
	m_inertiaTensor = m_inverseInertiaTensor = Matrix::Identity;
}

void RigidbodyComponent::Begin()
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

const Vector3& RigidbodyComponent::GetAngularVelocity() const
{
	return m_angularVelocity;
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
