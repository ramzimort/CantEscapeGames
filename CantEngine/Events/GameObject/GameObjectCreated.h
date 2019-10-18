#pragma once

#include "Events/Event.h"
#include "GameObjects/GameObject.h"

class GameObjectCreated : public Event<GameObjectCreated>
{
public:
	GameObjectCreated(size_t id, GameObject* go) :
		m_ID(id), m_pGameObject(go)
	{
	}
	virtual ~GameObjectCreated() { }

	size_t m_ID;
	GameObject* m_pGameObject;
};