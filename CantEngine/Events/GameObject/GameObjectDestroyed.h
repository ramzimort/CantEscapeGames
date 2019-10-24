#pragma once

#include "Events/Event.h"
#include "GameObjects/GameObject.h"

class GameObjectDestroyed : public Event<GameObjectDestroyed>
{
public:
	GameObjectDestroyed(size_t id, GameObject* pGameObject) :
		m_ID(id), m_pGameObject(pGameObject)
	{
	}
	virtual ~GameObjectDestroyed() { }
	virtual void operator()() override
	{

	}

	GameObject* m_pGameObject;
	size_t m_ID;
};