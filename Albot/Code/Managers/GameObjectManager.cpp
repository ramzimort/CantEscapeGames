/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "GameObjectManager.h"
#include "GameObjects/GameObject.h"

//TODO - delete when replacing for a message
#include "SystemManager.h"


GameObjectManager::GameObjectManager(SystemManager *sysMgr)
{
	this->m_systemMgr = sysMgr;
}

GameObjectManager::~GameObjectManager()
{
	//If the goMgr is getting destroyed, it forcefully destroys all gameObjs too
	for (auto &node : m_gameObjects) 
	{
		delete node.second;
	}
	m_gameObjects.clear();
}

void GameObjectManager::Queue_GameObject_Instantiation(GameObjectDesc *goDesc)
{
	m_instantiationQueue.push(*goDesc);
}

void GameObjectManager::Queue_GameObject_Destruction(size_t go_id)
{
	m_destructionQueue.push(go_id);
}

//Once a frame, this method will empty the instantiaion and destroy queues
void GameObjectManager::ProcessQueues() 
{
	//Destroy all objs
	Destroy_Queued_GameObjects();

	//Instantiate all objs
	Instantiate_Queued_GameObjects();
}

//TODO - When replacing for message, we can remove the sysMgr param
void GameObjectManager::Instantiate_Queued_GameObjects() 
{
	while (!m_instantiationQueue.empty())
	{
		//Dequeue the elements of the queue
		GameObjectDesc descriptor = m_instantiationQueue.front();
		m_instantiationQueue.pop();

		//Create the new GameObject
		GameObject *go = descriptor.tag == "" ? new GameObject(this) : new GameObject(this, descriptor.tag);
		if (go) 
		{
			descriptor.initializeComponentSetup(go);
			//Register on the system
			//TODO - REPLACE WITH MESSAGE TO SYSTEM MGR (for testing that)
			m_systemMgr->RegisterGameObject(go);

			//Finally, add to map of gameObjects
			this->m_gameObjects[go->GetId()] = go;
		}
	}
}

void GameObjectManager::Destroy_Queued_GameObjects() 
{
	while (!m_destructionQueue.empty())
	{
		//Dequeue the elements of the queue
		size_t go_id = m_destructionQueue.front();
		m_destructionQueue.pop();

		//Destroy the GameObject
		GameObject *go = m_gameObjects[go_id];
		if (go)
		{
			//Take out of the map
			m_gameObjects.erase(go_id);

			//Unsuscribe from system
			m_systemMgr->Unregister_GameObject(go_id);

			//Delete
			delete go;
		}
	}
}
