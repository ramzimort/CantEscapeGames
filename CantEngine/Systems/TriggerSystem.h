
#pragma once

///Includes
#include "BaseSystem.h"
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Managers/EventManager.h"
#include "Events/GameObject/GameObjectEvents.h"

class TransformComponent;
class TriggerComponent;
class AppRenderer;

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

	void LateUpdate(float dt) override;

	const DynamicAabbTree& GetTrigersTree();

	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;

	DynamicAabbTree m_triggers;
	QueryResults m_lastFrameQueries;

	void OnObjectDeleted(const GameObjectDestroyed* e);
};