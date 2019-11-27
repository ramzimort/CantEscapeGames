/**
 * @file RigidbodyComponent.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of the rigidbody 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

#include "BaseComponent.h"
#include "Physics/Geometry/Aabb.h"
#include "Physics/CollisionTable.h"
#include "Physics/Constraint.h"
#include "Events/Multicast.h"

class GameObjectManager;

class Contact;
class CollisionManifold;

/**
 * @brief Rigidbody component
 * 
 */
class RigidbodyComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class RigidbodySystem;
	friend class ScriptingManager;
//Public interface
public:
	RigidbodyComponent(GameObject *owner);
	virtual ~RigidbodyComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;

	// getters/setters
	/**
	 * @brief Get the Position of the object in the world space
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetPosition() const;

	/**
	 * @brief Get the Aabb of the object in a world space
	 * 
	 * @return const Aabb& 
	 */
	const Aabb& GetAabb() const;	
	/**
	 * @brief Get current linear velocity
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetVelocity() const;
	/**
	 * @brief Set linear Velocity of the object
	 * 
	 * @param velocity 
	 */
	void SetVelocity(const Vector3& velocity);
	/**
	 * @brief Get the Angular Velocity of the object
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetAngularVelocity() const;
	/**
	 * @brief Set the Angular Velocity of hte object object
	 * 
	 * @param angularVelocity 
	 */
	void SetAngularVelocity(const Vector3& angularVelocity);
	/**
	 * @brief Get the Mass of the object
	 * 
	 * @return float 
	 */
	float GetMass() const;
	/**
	 * @brief Set the Mass of the object
	 * 
	 * @param mass 
	 */
	void SetMass(float mass);
	/**
	 * @brief Get the collision mask of the object
	 * 
	 * @return CollisionTable::CollisionMask 
	 */
	CollisionTable::CollisionMask GetCollisionMask();
	/**
	 * @brief Checks if the object is aslepe
	 * 
	 * @return true 
	 * @return false 
	 */
	bool IsAsleep() const;
	
public:
	//Unique class identifier
	static ComponentId const static_type;

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

	float m_frictionCoef;
	
	CollisionTable::CollisionMask m_collisionMask;

	unsigned int m_dynamicAabbTreeKey;

	void InitInertiaTensor(float x, float y, float z);
	
	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
