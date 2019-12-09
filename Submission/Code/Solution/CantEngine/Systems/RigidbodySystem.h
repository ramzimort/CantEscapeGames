/**
 * @file RigidbodySystem.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief System that controls all the rigidbodies in the engine, most of the physics happens here 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

///Includes
#include "BaseSystem.h"
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Physics/CollisionTable.h"
#include "../Components/MeshComponent.h"
#include "Physics/Gjk/CollisionManifold.h"
#include "Managers/EventManager.h"
#include"Events/Input/KeyEvent.h"
#include "Events/GameObject/GameObjectEvents.h"

class TransformComponent;
class RigidbodyComponent;
class MeshComponent;
class AppRenderer;
/**
 * @brief Rigidbody component node, for fast access of transform, mesh, rigidbody
 * 
 */
struct RigidbodyCompNode : BaseSystemCompNode
{
	TransformComponent* m_transform;
	RigidbodyComponent* m_rigidbody;
	MeshComponent* m_mesh;

	//Ctor
	RigidbodyCompNode(TransformComponent* transform, RigidbodyComponent* rigidbody, MeshComponent* mesh)
	: m_transform(transform), m_rigidbody(rigidbody), m_mesh(mesh)
	{}
};
/**
 * @brief Controls all the rigidbodies
 * 
 */
class RigidbodySystem : public BaseSystem
{
//Public interface
public:
	RigidbodySystem();
	virtual ~RigidbodySystem();

	virtual void Register_GameObject(GameObject *go) override;
	void RegisterAppRenderer(AppRenderer* renderer);

	/**
	 * @brief Physics Update function
	 * 
	 * @param dt 
	 */
	virtual void LateUpdate(float dt) override;

	//void UpdateBoundingBox(const TransformComponent& transform, const Model);
protected:
	//protected vars

public:
	//To compare when using templates
	static unsigned int const static_type;

	/**
	 * @brief Get the dynamic Aabb tree that is used for broad phase of the collision detection
	 * 
	 * @return DynamicAabbTree& 
	 */
	DynamicAabbTree& GetAabbTree();
	/**
	 * @brief proccesses some key presses, such as pause, step to next frame(while paused)
	 * 
	 * @param keyEvent 
	 */
	void OnKeyDown(const KeyEvent* keyEvent);

private:
	// this is used to make fixed update time

	DynamicAabbTree m_broadPhase;
		
	CollisionTable m_collisionTable;
	AppRenderer* m_pAppRenderer;

	float  m_timeAccumulator;

	std::vector<ContactManifold> m_contactManifolds; // each contact manifold discribes all contacts between 2 colliding objects
	
	void ProcessCollision(Contact& collision);
	void RemoveCollision(const Contact& collision);

	void OnObjectDeleted(const GameObjectDestroyed* e);

#ifdef DEVELOPER
	bool m_isPaused;
	bool m_isAdvanceOneStep;
#endif
};