/**
 * @file TriggerComponent.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Trigger component to detect non physical collisions
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
 #pragma once

#include "BaseComponent.h"
#include "Physics/Geometry/Aabb.h"
#include "Events/Multicast.h"
#include "Physics/CollisionTable.h"

class GameObjectManager;
/**
 * @brief Trgger component
 * 
 */
class TriggerComponent : public BaseComponent
{
public:
	friend class Factory;
	friend class TriggerSystem;
	friend class ScriptingManager;
public:
	TriggerComponent(GameObject* owner);
	virtual ~TriggerComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager* goMgr) override;

	/**
	 * @brief Get the bounding box that discribes the trigger volume in the world space as if it was placed at the origin
	 * 
	 * @return const Aabb& 
	 */
	const Aabb& GetAabb() const;
	/**
	 * @brief Get the Scale of the trigger. Note that this is treated separetly from objects transform scale and used as it is in the world space
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetScale();
	/**
	 * @brief Set the Scale of the trigger
	 * 
	 * @param scale 
	 */
	void SetScale(const Vector3& scale);
	/**
	 * @brief Get the Offset position of the trigger from the objects translation
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetOffset();
	/**
	 * @brief Set the Offset of the trigger
	 * 
	 * @param offset 
	 */
	void SetOffset(const Vector3& offset);
	/**
	 * @brief Get the Collision Mask of the trigger. Note: this is separate from the collision mask on the rigidbody, because object does not have to have rigidbody
	 * 
	 * @return CollisionTable::CollisionMask 
	 */
	CollisionTable::CollisionMask GetCollisionMask();
	void SetCollisionMask(CollisionTable::CollisionMask mask);

public:
	// Unique class identifier
	static ComponentId const static_type;

	Multicast<void(GameObject*, GameObject*)> m_onEnter;
	Multicast<void(GameObject*, GameObject*)> m_onExit;
private:
	Aabb m_aabb; 
	Vector3 m_scale;
	Vector3 m_offset; // offset from the transform
	CollisionTable::CollisionMask m_collisionMask;

	unsigned int m_dynamicAabbTreeKey;

	void UpdateAabb();

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
