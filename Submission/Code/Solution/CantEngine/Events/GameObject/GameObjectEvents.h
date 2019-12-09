#pragma once

#include "Events/Event.h"
#include "GameObjects/GameObject.h"

/**
 * @file GameObjectEvents.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Game Object Events
 */

/**
 * @brief Pass when you want to destroy a game object and you know it's ID
 * 
 */
class DestroyGameObject : public Event<DestroyGameObject>
{
public:
	/**
	 * @brief Pass when you want to destroy a game object and you know it's ID
	 * 
	 * @param id 
	 */
	DestroyGameObject(size_t id) :
		m_ID(id) { }
	virtual ~DestroyGameObject() { }
	virtual void operator()() override
	{

	}
    /**
     * @brief Game Object ID
     * 
     */
	size_t m_ID;
};

/**
 * @brief Broadcasted when a game object is created (mainly used for DEVELOPER)
 * 
 */
class GameObjectCreated : public Event<GameObjectCreated>
{
public:
	/**
	 * @brief Broadcasted when a game object is created (mainly used for DEVELOPER)
	 * 
	 * @param id 
	 * @param go 
	 */
	GameObjectCreated(size_t id, GameObject* go) :
		m_ID(id), m_pGameObject(go)
	{
	}
	virtual ~GameObjectCreated() { }
	virtual void operator()() override { }

    /**
     * @brief Game Object ID
     * 
     */
	size_t m_ID;

    /**
     * @brief Game Object Pointer
     * 
     */
	GameObject* m_pGameObject;
};

/**
 * @brief Broadcasted when a game object is destroyed (Mainly used for DEVELOPER)
 * 
 */
class GameObjectDestroyed : public Event<GameObjectDestroyed>
{
public:
	/**
	 * @brief Broadcasted when a game object is destroyed (Mainly used for DEVELOPER)
	 * 
	 * @param id 
	 * @param pGameObject 
	 */
	GameObjectDestroyed(size_t id, GameObject* pGameObject) :
		m_ID(id), m_pGameObject(pGameObject)
	{
	}
	virtual ~GameObjectDestroyed() { }
	virtual void operator()() override
	{

	}

    /**
     * @brief Game Object ID
     * 
     */
	size_t m_ID;

    /**
     * @brief Game Object Pointer
     * 
     */
	GameObject* m_pGameObject;
};