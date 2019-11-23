/**
 * @file State.h
 * @author Jose Rosenbluth
 * @date 12/4/2019
 * @brief Represent a game state
 */


#pragma once

class Factory;

//Check which managers the state has
class GameObjectManager;
class SystemManager;
class AppRenderer;
class ResourceManager;
class ScriptingManager;
class ResourcesLoadedEvent;


/**
 * @brief Represent a game state. Each state will have their own GameObjectManager and SystemManager
 * 
 */
class State
{
public:
	/**
	 * @brief Construct a new State object
	 * 
	 * @param path 
	 * @param appRenderer 
	 * @param resMgr 
	 * @param luaMgr 
	 */
	State(std::string const& path, AppRenderer* appRenderer, 
		ResourceManager* resMgr, ScriptingManager *luaMgr);
	/**
	 * @brief Destroy the State object
	 * 
	 */
	virtual ~State();
	/**
	 * @brief Event fired when resources are loaded
	 * 
	 * @param e 
	 */
	void OnResourcesLoaded(const ResourcesLoadedEvent* e);


	/**
	 * @brief Calls Update on the system Manager belonging to the state
	 * 
	 * @param dt 
	 */
	virtual void Update(float dt);

	/**
	 * @brief Calls draw on the system Manager belonging to the state
	 * 
	 * @param dt 
	 */
	virtual void Draw(float dt);

	/**
	 * @brief Calls ProcessQueues on the GameObjectManager belonging to this state
	 * 
	 */
	void ProcessInstantiationAndDestruction();

private:
	/**
	* @brief Scripted call to OnExitState
	* 
	*/
	void CallOnExitState();

private:
	/**
	 * @brief Factory pointer used by the state to create its GameObjects
	 * 
	 */
	Factory *m_factory;
	/**
	 * @brief GameObjectManager. Created and owned by the state
	 * 
	 */
	GameObjectManager *m_gameObjectMgr;
	/**
	 * @brief SystemManager. Created and owned by the state
	 * 
	 */
	SystemManager *m_systemMgr;

	/**
	 * @brief Pointer to the Graphic's Manager
	 * 
	 */
	AppRenderer *m_pRenderer;
	/**
	 * @brief Pointer to the Resource Manager
	 * 
	 */
	ResourceManager *m_pResMgr;
	/**
	 * @brief Pointer to the Scripting Manager
	 * 
	 */
	ScriptingManager* m_pLuaMgr;
	/**
	 * @brief Path to this state json file
	 * 
	 */
	std::string m_path;
	/**
	 * @brief Lua table that representes the script ran by this state
	 * 
	 */
	sol::table m_stateScript;
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
};

