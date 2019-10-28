/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#pragma once

class Factory;

//Check which managers the state has
class GameObjectManager;
class SystemManager;
class AppRenderer;
class ResourceManager;
class ScriptingManager;
class ResourcesLoadedEvent;

class State
{
public:
	State(std::string const& path, AppRenderer* appRenderer, 
		ResourceManager* resMgr, ScriptingManager *luaMgr);
	virtual ~State();
	void OnResourcesLoaded(const ResourcesLoadedEvent* e);


	//Call update on systemManager
	virtual void Update(float dt);

	//Call draw on systemManager
	virtual void Draw(float dt);

	//Calls the gameObjMgr to check its queues
	void ProcessInstantiationAndDestruction();

private:
	//Scripted calls
	///void CallOnEnterState();
	void CallOnExitState();

private:
	Factory *m_factory;
	GameObjectManager *m_gameObjectMgr;
	SystemManager *m_systemMgr;

	// For the init call, I need the state to hold 
	// a ptr to the resMgr and the renderer
	AppRenderer *m_pRenderer;
	ResourceManager *m_pResMgr;
	ScriptingManager* m_pLuaMgr;
	std::string m_path;

	sol::table m_stateScript;
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
};

