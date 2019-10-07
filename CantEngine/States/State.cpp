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
	m_gameObjectMgr = new GameObjectManager(m_systemMgr);
	m_factory = new Factory(path, m_gameObjectMgr, m_systemMgr, resMgr, appRenderer->GetDXRenderer(), luaMgr);

	//Loads a state based on the json file on path

	// A state has
	//		1- A gameobject manager
	//		2- A system manager
	//		3- A physics manager?
	//		4- A memory stack Marker (where the stack should reset to when it deallocates)

	// TODO - do actual loading. For now, just creates a factory with hardcoded stuff in it
}

State::~State() 
{
	//Deallocates its managers
	delete m_gameObjectMgr;
	delete m_systemMgr;

	//Deletes the factory obj
	delete m_factory;
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
	this->m_gameObjectMgr->ProcessQueues();
}