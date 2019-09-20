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


State::State(std::string const& path)
{
	//Allocates its managers
	m_systemMgr = new SystemManager();
	m_gameObjectMgr = new GameObjectManager(m_systemMgr);

	//Loads a state based on the json file on path

	// A state has
	//		1- A gameobject manager
	//		2- A system manager
	//		3- A physics manager?

	// TODO - do actual loading. For now, just creates a factory with hardcoded stuff in it
	this->m_factory = new Factory(path, m_gameObjectMgr, m_systemMgr);
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
	this->m_systemMgr->Update(dt);
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