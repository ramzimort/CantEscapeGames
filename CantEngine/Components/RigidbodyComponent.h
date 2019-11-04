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
#include "Events/Multicast.h"

class GameObjectManager;

class Contact;
class CollisionManifold;

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

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;

	// getters/setters
	const Vector3& GetPosition() const;
	const Aabb& GetAabb() const;

	const Vector3& GetVelocity() const;
	void SetVelocity(const Vector3& velocity);

	const Vector3& GetAngularVelocity() const;
	void SetAngularVelocity(const Vector3& angularVelocity);

	float GetMass() const;
	void SetMass(float mass);

	CollisionTable::CollisionMask GetCollisionMask();

	bool IsAsleep() const;
	
public:
	//Unique class identifier
	static ComponentId const static_type;

	//TODO - Remove before push
	Multicast<void(GameObject*, GameObject*)> m_onCollision;

private:
	Aabb m_aabb;

	Vector3 m_position;
	Vector3 m_velocity;
	Quaternion m_quaternion;
	Vector3 m_angularVelocity;

	Matrix m_inertiaTensor;
	Matrix m_inertiaTensorInverse;
	Matrix m_inertiaTensorWorldInverse;

	Vector3 m_forceAccumulator;
	Vector3 m_torqueAcumulator;

	float m_mass;
	float m_inverseMass;

	bool m_isEffectedByGravity;
	bool m_isAsleep;
	bool m_isColliding;
	
	CollisionTable::CollisionMask m_collisionMask;

	unsigned int m_dynamicAabbTreeKey;

	void InitInertiaTensor(float x, float y, float z);
	
	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
