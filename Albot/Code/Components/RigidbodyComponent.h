/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"
#include "Physics/Geometry/Aabb.h"
#include "Physics/CollisionTable.h"
#include "Physics/Constraint.h"

class Constraint;
class RigidbodyComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class RigidbodySystem;
//Public interface
public:
	RigidbodyComponent(GameObject *owner);
	virtual ~RigidbodyComponent();

	virtual void Init(ResourceManager* resMgr) override;
	virtual void Begin() override;

	// getters/setters
	const Vector3& GetPosition() const;
	const Aabb& GetAabb() const;
	const Vector3& GetVelocity() const;
	const Vector3& GetAngularVelocity() const;

	float GetMass() const;
	void SetMass(float mass);
	
public:
	//Unique class identifier
	static ComponentId const static_type;
	
private:
	Aabb m_aabb;

	Vector3 m_position;
	Vector3 m_velocity;
	Quaternion m_quaternion;
	Vector3 m_angularVelocity;

	Matrix m_inertiaTensor;
	Matrix m_inverseInertiaTensor;
	Matrix m_inertiaTensorWorldInverse;
	float m_mass;
	float m_inverseMass;

	bool m_isEffectedByGravity;
	
	std::vector<Constraint> m_constraints;
	
	CollisionTable::CollisionMask m_collisionMask;

	unsigned int m_dynamicAabbTreeKey;
	
	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
