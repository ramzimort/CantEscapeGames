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
	for (auto& node : systems)
		delete node.second;
	systems.clear();
}


bool SystemManager::RegisterGameObject(GameObject *go) 
{
	for (auto& node : systems)
	{
		BaseSystem *system = node.second;
		if (system->Can_Register_GameObject(go))
			return true;
	}

	return false;
}


void SystemManager::Unregister_GameObject(size_t go_id)
{
	for (auto& node : systems)
	{
		BaseSystem *system = node.second;
		system->Unregister_GameObject(go_id);
	}
}


void SystemManager::Update(float dt)
{
	for (auto& node : systems) 
	{
		BaseSystem *system = node.second;
		system->UpdateAllNodes(dt);
	}
}


void SystemManager::Draw(float dt)
{
	for (auto& node : systems)
	{
		BaseSystem *system = node.second;
		system->DrawAllNodes(dt);
	}
}