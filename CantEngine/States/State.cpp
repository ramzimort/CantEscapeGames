/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "State.h"
#include "../Factory/Factory.h"

//Check which managers the state has
#include "../Managers/SystemManager.h"
#include "../Managers/GameObjectManager.h"
#include "Graphics/AppRenderer.h"
#include "../Managers/ScriptingManager.h"



State::State(std::string const& path, AppRenderer* appRenderer, 
	ResourceManager* resMgr, ScriptingManager *luaMgr)
{
	//Allocates its managers
	m_systemMgr = new SystemManager(appRenderer);
	m_gameObjectMgr = new GameObjectManager(m_systemMgr, luaMgr);
	Factory::LoadLevel(path, m_gameObjectMgr);

	//For calling init from the instantiate, need to save these two
	this->m_pRenderer = appRenderer;
	this->m_pResMgr = resMgr;

	// A state has
	//		1- A gameobject manager
	//		2- A system manager
	//		3- A physics manager?
	//		4- A memory stack Marker (where the stack should reset to when it deallocates)
	//		5- A script to determine variables and make each state have their own context

	// Script setup
	try
	{
		//Get the correct script name from the path
		size_t index = path.find_last_of("/\\");
		size_t len = (path.size() - 5) - (index + 1);
		std::string name = "Scripts/States/" + path.substr(index + 1, len) + ".lua";

		//Get a deep copy of the table so each table has their own state
		//  For now, to check if the table is valid for calling, 
		//  we can check if the luastate associated is null
		//  Either way, since the lua calls are protected, nothing should crash
		m_stateScript = luaMgr->GetScriptDeepCopy(name);

		m_stateScript["OnCreateState"](m_stateScript);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName); //TODO - erase this
	}
}

State::~State() 
{
	//Call the script for clearing up stuff there
	CallOnExitState();

	//Deallocates its managers
	delete m_gameObjectMgr;
	delete m_systemMgr;
}


//Call update on systemManager
void State::Update(float dt)
{
	this->m_systemMgr->UpdateSystems(dt);
}

//Call draw on systemManager
void State::Draw(float dt)
{
	this->m_systemMgr->Draw(dt);
}

//Calls the gameObjMgr to check its queues
void State::ProcessInstantiationAndDestruction()
{
	//Passing the renderer and resMgr ptr to call init from goMgr
	this->m_gameObjectMgr->ProcessQueues(m_pRenderer, m_pResMgr);
}


// Called when the state is being destroyed
void State::CallOnExitState()
{
	try
	{
		m_stateScript["OnExitState"](m_stateScript);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName); //TODO - erase this
	}
}