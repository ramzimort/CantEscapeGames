/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "SystemManager.h"
#include "Systems/BaseSystem.h"

//For now, all custom system headers
#include "Systems/RenderingSystem.h"
#include "Systems/RigidbodySystem.h"
#include "Systems/TestSystem.h"


SystemManager::SystemManager()
{
	//ADD HERE THE CUSTOM SYSTEMS
	this->AddSystem<RigidbodySystem>();
	this->AddSystem<RenderingSystem>();

	//Test (erase later)
	this->AddSystem<TestSystem>();
}

SystemManager::~SystemManager()
{
	for (auto& node : m_systems)
		delete node.second;
	m_systems.clear();
}


bool SystemManager::RegisterGameObject(GameObject *go) 
{
	for (auto& node : m_systems)
	{
		BaseSystem *system = node.second;
		if (system->Can_Register_GameObject(go))
			return true;
	}

	return false;
}


void SystemManager::Unregister_GameObject(size_t go_id)
{
	for (auto& node : m_systems)
	{
		BaseSystem *system = node.second;
		system->Unregister_GameObject(go_id);
	}
}


void SystemManager::UpdateSystems(float dt)
{
	for (auto& node : m_systems) 
	{
		BaseSystem *system = node.second;

		//Early update (only once per system)
		system->EarlyUpdate(dt);

		// Regular update (once per object registered in the system)
		system->UpdateAllNodes(dt);

		//Late update (only once per system)
		system->LateUpdate(dt);
	}
}


void SystemManager::Draw(float dt)
{
	for (auto& node : m_systems)
	{
		BaseSystem *system = node.second;
		system->DrawAllNodes(dt);
	}
}