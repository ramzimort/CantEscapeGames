/**
 * @file GameObjectManager.h
 * @author Jose Rosenbluth
 * @date 12/4/2019
 * @brief Manager that handles creation and destruction of every GameObject in the current game state
 */

#pragma once

class GameObject;
class SystemManager;
class ScriptingManager;
class AppRenderer;
class ResourceManager;
class DestroyGameObject;

/**
 * @brief Descriptor of a GameObject. Used when deferring creation of new instances,
  to store the information wanted on instantiation.
 * 
 */
struct GameObjectDesc 
{
	/**
	 * @brief Typedef of lambda function pointer
	 */
	using InitializeComponentSetup = std::function<void(GameObject*)>;
	
	/**
	 * @brief GameObject's tag
	 */
	std::string tag;

	/**
	 * @brief Prefab name, used when trying to instantiate from an existing prefab
	 */
	std::string prefabName;

	/**
	 * @brief Function pointer that holds the lambda that will be ran 
	 when instantiating the GameObject
	 */
	InitializeComponentSetup initializeComponentSetup;

	/**
	 * @brief Construct a new Game Object Desc object
	 */
	GameObjectDesc() :
		tag(""),
		initializeComponentSetup([](GameObject*) {})
	{}
};


/**
 * @brief Manager that handles creation and destruction of every GameObject in the current game state
 * 
 */
class GameObjectManager
{
public:
	/**
	 * @brief Construct a new Game Object Manager object
	 * 
	 * @param sysMgr System Manager, passed to register new GameObjects to the required systems
	 * @param luaMgr Scripting Manager, passed to setup the scripting components for the GameObject
	 */
	GameObjectManager(SystemManager *sysMgr, ScriptingManager *luaMgr);
	/**
	 * @brief Destroy the Game Object Manager object
	 * 
	 */
	~GameObjectManager();

	/**
	 * @brief Process the instantiation and destruction queues. Called once per frame (start of frame)
	 * 
	 * @param pRenderer Graphics Manager
	 * @param pResMgr Resource Manager
	 */
	void ProcessQueues(AppRenderer *pRenderer, ResourceManager *pResMgr);

	/**
	 * @brief Queues a GameObject descriptor so later a new GameObject can be instantiated based on it
	 *
	 * @param goDesc 
	 */
	void Queue_GameObject_Instantiation(GameObjectDesc *goDesc);
	/**
	 * @brief Queues an ID, so the GameObject represented by it can be destroyed in the next call to ProcessQueues
	 * 
	 * @param go_id 
	 */
	void Queue_GameObject_Destruction(size_t go_id);
	/**
	 * @brief Queues a DestroyGameObject for destruction
	 * 
	 * @param e 
	 */
	void Queue_GameObject_DestructionE(const DestroyGameObject* e);
	/**
	 * @brief Adds a GameObject created in the script to a instantiation queue, which will be emptied in the next call to Instantiate_Queued_GameObjects
	 * 
	 * @param scriptedGO 
	 */
	void AddToScriptInstantiateQueue(GameObject *scriptedGO);

	/**
	 * @brief Returns, if it exist, a pointer to the GameObject with unique ID id
	 * 
	 * @param id 
	 * @return GameObject* 
	 */
	GameObject *FindGameObjectById(size_t id);
	/**
	 * @brief Returns, if it exists, a pointer to the GameObject with TAG tag
	 * 
	 * @param tag 
	 * @return GameObject* 
	 */
	GameObject *FindGameObject(std::string const& tag);

	/**
	 * @brief Get the Scripting Manager object that we hold a pointer to (not owned by the GameObjectManager)
	 * 
	 * @return ScriptingManager* 
	 */
	ScriptingManager *GetScriptingManager() const;

private:
	/**
	 * @brief Called from ProcessQueues. It will instantiate and setup the GameObjects queued as descriptors, and 
	 will handle setup for the GameObjects created from the script
	 * 
	 * @param pRenderer 
	 * @param pResMgr 
	 */
	void Instantiate_Queued_GameObjects(AppRenderer *pRenderer, ResourceManager *pResMgr);
	/**
	 * @brief After a GameObject is instantiated, it is sent here to complete its setup
	 * 
	 * @param go 
	 * @param pRenderer 
	 * @param pResMgr 
	 * @param newGameObjects 
	 */
	void CompleteGORegistration(GameObject *go, AppRenderer *pRenderer, ResourceManager *pResMgr,
		std::vector<GameObject*>& newGameObjects);
	/**
	 * @brief After Instantiating all gameobjects this frame, this will call Begin on them
	 * 
	 * @param newGameObjects 
	 */
	void CallBeginOnNewInstances(std::vector<GameObject*> const& newGameObjects);
	/**
	 * @brief Called by ProcessQueues. Will destroy the queued GameObjects for destruction
	 * 
	 */
	void Destroy_Queued_GameObjects();

#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
///////////////
// VARIABLES //
///////////////
private:
	/**
	 * @brief Main container for gameobjects. Gameobjects here are in the game and registered in systems
	 * 
	 */
	std::unordered_map<size_t, GameObject*> m_gameObjects;

	/**
	 * @brief Map to find tagged gameobjs. If a gameobj has a duplicate tag, it wont be added!
	 * 
	 */
	std::unordered_map<std::string, GameObject*> m_taggedGameObjs;

	/**
	 * @brief Container for gameobjects created by scripts, which will be added to the game next frame
	 * 
	 */
	std::vector<GameObject*> m_scriptedInstances;
	
	/**
	 * @brief Queues where info about objs to be created or destroyed next frame is stored
	 * 
	 */
	std::queue<GameObjectDesc> m_instantiationQueue;
	/**
	 * @brief Queue containing ID for all the GameObjects to be destroyed in the next frame call to ProcessQueues
	 * 
	 */
	std::queue<size_t> m_destructionQueue;

	/**
	 * @brief System manager pointer (to register gameobjects)
	 * 
	 */
	SystemManager *m_systemMgr;
	
	/**
	 * @brief Scripting manager pointer (to register scripted components)
	 * 
	 */
	ScriptingManager *m_luaMgr;
};