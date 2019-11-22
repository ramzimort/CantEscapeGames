#pragma once

///INCLUDES
#include "BaseComponent.h"
#include "Physics/Geometry/Aabb.h"
#include "Events/Multicast.h"
#include "Physics/CollisionTable.h"

class GameObjectManager;
class TriggerComponent : public BaseComponent
{
	//Friend classes
public:
	friend class Factory;
	friend class TriggerSystem;
	friend class ScriptingManager;
	//Public interface
public:
	TriggerComponent(GameObject* owner);
	virtual ~TriggerComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager* goMgr) override;

	const Aabb& GetAabb() const;

	const Vector3& GetScale();
	void SetScale(const Vector3& scale);

	const Vector3& GetOffset();
	void SetOffset(const Vector3& offset);

	CollisionTable::CollisionMask GetCollisionMask();

public:
	// Unique class identifier
	static ComponentId const static_type;

	Multicast<void(GameObject*, GameObject*)> m_onCollisionEnter;
	Multicast<void(GameObject*, GameObject*)> m_onCollisionExit;
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
