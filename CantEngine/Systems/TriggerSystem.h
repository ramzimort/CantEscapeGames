/**
 * @file TriggerSystem.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Controls all the triggers in the engine
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
#pragma once

///Includes
#include "BaseSystem.h"
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Managers/EventManager.h"
#include "Events/GameObject/GameObjectEvents.h"

class TransformComponent;
class TriggerComponent;
class AppRenderer;

/**
 * @brief Stores pointer to a transform and trigger to fast access
 * 
 */
struct TriggerCompNode : BaseSystemCompNode
{
	TransformComponent* m_transform;
	TriggerComponent* m_trigger;

	//Ctor
	TriggerCompNode(TransformComponent* transform, TriggerComponent* trigger)
		: m_transform(transform), m_trigger(trigger)
	{}
};

class TriggerSystem : public BaseSystem
{
public:
	TriggerSystem();
	~TriggerSystem();

	virtual void Register_GameObject(GameObject* go) override;

	void RegisterAppRenderer(AppRenderer* renderer);

	/**
	 * @brief All the trigger components getting updated here
	 * 
	 * @param dt 
	 */
	void LateUpdate(float dt) override;

	/**
	 * @brief Get the dynamic aabb Tree that stores all the triggers 
	 * 
	 * @return const DynamicAabbTree& 
	 */
	const DynamicAabbTree& GetTrigersTree();

	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;

	DynamicAabbTree m_triggers;
	QueryResults m_lastFrameQueries;

	void OnObjectDeleted(const GameObjectDestroyed* e);
};