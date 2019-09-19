/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>

class BaseSystem;
class GameObject;


class SystemManager
{
public:
	SystemManager();
	~SystemManager();

	void Update(float dt);
	bool RegisterGameObject(GameObject *go);
	void Unregister_GameObject(size_t go_id);
	
	template<typename T>
	void AddSystem();

private:

	//For now, this will have a vector of systems
	std::unordered_map<unsigned int, BaseSystem*> systems;
};


template<typename T>
void SystemManager::AddSystem() 
{
	unsigned int SystemTypeId = T::static_type;

	//If the system has not been created
	if (systems.count(SystemTypeId) == 0) 
	{
		T* system = new T();

		if (system)
			systems[SystemTypeId] = system;
	}
}