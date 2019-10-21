/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class GameObject;
class SystemManager;
class ScriptingManager;
class AppRenderer;
class ResourceManager;
class DestroyGameObject;

struct GameObjectDesc 
{
	using InitializeComponentSetup = std::function<void(GameObject*)>;
	
	std::string tag;
	InitializeComponentSetup initializeComponentSetup;

	GameObjectDesc() :
		tag(""),
		initializeComponentSetup([](GameObject*) {})
	{}
};


class GameObjectManager
{

/////////////
// METHODS //
/////////////
public:
	GameObjectManager(SystemManager *sysMgr, ScriptingManager *luaMgr);
	~GameObjectManager();

	//For now, the once a frame method that will process the queues
	void ProcessQueues(AppRenderer *pRenderer, ResourceManager *pResMgr);

	void Queue_GameObject_Instantiation(GameObjectDesc *goDesc);
	void Queue_GameObject_Destruction(size_t go_id); 
	void Queue_GameObject_DestructionE(const DestroyGameObject* e);
	void AddToScriptInstantiateQueue(GameObject *scriptedGO);

	GameObject *FindGameObjectById(size_t id);
	GameObject *FindGameObject(std::string const& tag);

	//Get LuaMgr (So gameobj can get them when trying to instantiate objs)
	ScriptingManager *GetScriptingManager() const;

private:
	void Instantiate_Queued_GameObjects(AppRenderer *pRenderer, ResourceManager *pResMgr);
	void CompleteGORegistration(GameObject *go, AppRenderer *pRenderer, ResourceManager *pResMgr,
		std::vector<GameObject*>& newGameObjects);
	void CallBeginOnNewInstances(std::vector<GameObject*> const& newGameObjects);
	void Destroy_Queued_GameObjects();

#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
///////////////
// VARIABLES //
///////////////
private:
	//Main container for gameobjects. Gameobjects here are in the game and registered in systems
	std::unordered_map<size_t, GameObject*> m_gameObjects;

	//Map to find tagged gameobjs. If a gameobj has a duplicate tag, it wont be added!
	std::unordered_map<std::string, GameObject*> m_taggedGameObjs;

	// TESTING - Container for gameobjects created by scripts, which will be added to the game next frame
	std::vector<GameObject*> m_scriptedInstances;
	
	//Queues where info about objs to be created or destroyed next frame is stored
	std::queue<GameObjectDesc> m_instantiationQueue;
	std::queue<size_t> m_destructionQueue;

	//System manager pointer (to register gameobjects)
	SystemManager *m_systemMgr;
	ScriptingManager *m_luaMgr;
};