/**
 * @file SystemManager.h
 * @author Jose Rosenbluth
 * @date 12/4/2019
 * @brief Main class managing the systems on the ECS architecture
 */

#pragma once

class BaseSystem;
class GameObject;
class AppRenderer;

/**
 * @brief Main class managing the systems on the ECS architecture
 */
class SystemManager
{
public:
	/**
	 * @brief Construct a new System Manager object
	 * 
	 * @param appRenderer 
	 */
	SystemManager(AppRenderer* appRenderer = nullptr);
	/**
	 * @brief Destroy the System Manager object
	 */
	~SystemManager();

	/**
	 * @brief Calls Update on every system owned by the manager
	 * 
	 * @param dt 
	 */
	void UpdateSystems(float dt);
	/**
	 * @brief Calls draw on every system owned by the manager
	 * 
	 * @param dt 
	 */
	void Draw(float dt);

	/**
	 * @brief Registers the go GameObject, so depending on which 
	 components it owns, it will be updated when the relevant systems are
	 * 
	 * @param go 
	 */
	void RegisterGameObject(GameObject *go);
	/**
	 * @brief Unregisters a GameObject go from the systems in which it
	 was previously registered
	 * 
	 * @param go_id 
	 */
	void Unregister_GameObject(size_t go_id);
	
	/**
	 * @brief Templatized method which handles the creation of new systems
	 * 
	 * @tparam T 
	 */
	template<typename T>
	void AddSystem();

private:

	/**
	 * @brief Hash map of all the systems owned by the manager
	 * 
	 */
	std::unordered_map<unsigned int, BaseSystem*> m_systems;
};


template<typename T>
void SystemManager::AddSystem() 
{
	unsigned int SystemTypeId = T::static_type;

	//If the system has not been created
	if (m_systems.count(SystemTypeId) == 0) 
	{
		T* system = new T();

		if (system)
			m_systems[SystemTypeId] = system;
	}
}