/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RigidbodyComponent.h"
unsigned const RigidbodyComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<RigidbodyComponent>("Rigidbody")
		.method("Init", &RigidbodyComponent::Init)
		.method("Begin", &RigidbodyComponent::Begin);
}

RigidbodyComponent::RigidbodyComponent(GameObject *owner) :
	BaseComponent(owner, RigidbodyComponent::static_type)
{
}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Init()
{
	m_inertiaTensor = m_inverseInertiaTensor = Matrix::Identity;
}

void RigidbodyComponent::Begin()
{
}

const Aabb& RigidbodyComponent::GetAabb() const
{
	return m_aabb;
}

const Vector3& RigidbodyComponent::GetVelocity() const
{
	return m_velocity;
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
