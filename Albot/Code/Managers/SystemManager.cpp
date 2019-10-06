/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "SystemManager.h"
#include "Systems/BaseSystem.h"

//For now, all custom system headers
#include "Systems/CustomSystem/CustomSystem.h"
#include "Systems/RenderingSystem.h"
#include "Systems/RigidbodySystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/TestSystem.h"
#include "Systems/ParticleEmitterSystem.h"
#include "Systems/LightSystem.h"
#include "Systems/CameraSystem.h"
#ifdef DEVELOPER
#include "Systems/TestSystems/FPSCameraSystem.h"
#endif // DEVELOPER


SystemManager::SystemManager(AppRenderer* appRenderer)
{
	this->AddSystem<CustomSystem>();
	this->AddSystem<TransformSystem>();
	this->AddSystem<RigidbodySystem>();
	this->AddSystem<CameraSystem>();
	this->AddSystem<LightSystem>();
	this->AddSystem<RenderingSystem>();
	this->AddSystem<ParticleEmitterSystem>();
	static_cast<LightSystem*>(m_systems[LightSystem::static_type])->RegisterAppRenderer(appRenderer);
	static_cast<RenderingSystem*>(m_systems[RenderingSystem::static_type])->RegisterAppRenderer(appRenderer);
	static_cast<RigidbodySystem*>(m_systems[RigidbodySystem::static_type])->RegisterAppRenderer(appRenderer);
	static_cast<ParticleEmitterSystem*>(m_systems[ParticleEmitterSystem::static_type])->RegisterAppRenderer(appRenderer);

#ifdef DEVELOPER
	this->AddSystem<FPSCameraSystem>();
#endif
}

SystemManager::~SystemManager()
{
	for (auto& node : m_systems)
		delete node.second;
	m_systems.clear();
}


void SystemManager::RegisterGameObject(GameObject *go) 
{
	for (auto& node : m_systems)
	{
		BaseSystem *system = node.second;
		system->Can_Register_GameObject(go);	
	}
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