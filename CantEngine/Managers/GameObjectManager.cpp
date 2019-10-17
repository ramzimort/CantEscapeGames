/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "GameObjectManager.h"
#include "GameObjects/GameObject.h"
#include "Memory/CantMemory.h"
#include "SystemManager.h"
#include "ScriptingManager.h"

//To call init on each go's components, we need to pass these two
#include "Graphics/AppRenderer.h"
#include "Managers/ResourceManager.h"


GameObjectManager::GameObjectManager(SystemManager *sysMgr, ScriptingManager *luaMgr)
{
	this->m_systemMgr = sysMgr;
	this->m_luaMgr = luaMgr;
}

GameObjectManager::~GameObjectManager()
{
	//If the goMgr is getting destroyed, it forcefully destroys all gameObjs too
	for (auto &node : m_gameObjects) 
	{
		const auto& it = m_taggedGameObjs.find(node.second->GetTag());
		if (it != m_taggedGameObjs.end())
			m_taggedGameObjs.erase(it);

		CantMemory::PoolResource<GameObject>::Free(node.second);
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
void GameObjectManager::ProcessQueues(AppRenderer *pRenderer, ResourceManager *pResMgr)
{
	//Destroy all objs
	Destroy_Queued_GameObjects();

	//Instantiate all objs
	Instantiate_Queued_GameObjects(pRenderer, pResMgr);
}

//TODO - When replacing for message, we can remove the sysMgr param
void GameObjectManager::Instantiate_Queued_GameObjects(AppRenderer *pRenderer, ResourceManager *pResMgr)
{
	//This will hold the new instances, so we 
	//can later call begin on all the new ones
	std::vector<GameObject*> newGameObjects;

	//First, go through the queue and create new instances
	while (!m_instantiationQueue.empty())
	{
		//Dequeue the elements of the queue
		GameObjectDesc& descriptor = m_instantiationQueue.front();

		//Create the new GameObject and handle tag
		GameObject *go = (descriptor.tag == "") ? new GameObject(this) : new GameObject(this, descriptor.tag);
		if (go) 
		{
			//Lambda call
			descriptor.initializeComponentSetup(go);

			//Registering for the new GO
			CompleteGORegistration(go, pRenderer, pResMgr,
				newGameObjects);
		}

		m_instantiationQueue.pop();
	}

	// Second, go through scriptedInstances
	for (GameObject *go : m_scriptedInstances)
		CompleteGORegistration(go, pRenderer, pResMgr, 
			newGameObjects);
	m_scriptedInstances.clear();


	//Finally, need to call begin on all new gameObjects
	//Only on those created this frame
	this->CallBeginOnNewInstances(newGameObjects);
}



void GameObjectManager::CompleteGORegistration(GameObject *go, AppRenderer *pRenderer, 
	ResourceManager *pResMgr, std::vector<GameObject*>& newGameObjects)
{
	//Call Init
	go->Init(pRenderer, pResMgr);

	//Check if the GO is tagged, and add it to the 
	std::string const& tag = go->GetTag();
	if (tag != "" && m_taggedGameObjs.find(tag) == m_taggedGameObjs.end())
		m_taggedGameObjs[tag] = go;

	//Suscribe to systems
	m_systemMgr->RegisterGameObject(go);

	//Add to the main list
	this->m_gameObjects[go->GetId()] = go;

	//Add to newgo list
	newGameObjects.push_back(go);
}



void GameObjectManager::CallBeginOnNewInstances(std::vector<GameObject*> const& newGameObjects) 
{
	for (GameObject *go : newGameObjects) 
	{
		go->Begin();
	}
}


GameObject *GameObjectManager::FindGameObjectById(size_t id)
{
	return m_gameObjects[id];
}


GameObject *GameObjectManager::FindGameObject(std::string const& tag)
{
	auto iter = m_taggedGameObjs.find(tag);
	if (iter != m_taggedGameObjs.end()) 
		return iter->second;
	return nullptr;
}

void GameObjectManager::AddToScriptInstantiateQueue(GameObject *scriptedGO)
{
	this->m_scriptedInstances.push_back(scriptedGO);
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


//Get LuaMgr (So gameobj can get them when trying to instantiate objs)
ScriptingManager *GameObjectManager::GetScriptingManager() const
{
	return this->m_luaMgr;
}