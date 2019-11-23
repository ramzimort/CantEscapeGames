/**
 * @file GameObject.h
 * @author your name (you@domain.com)
 * @brief Class representing the most basic game unit on our engine
 * @date 2019-11-21
 */

#pragma once

#include <rttr/visitor.h>
#include "Components/BaseComponent.h"
#include "Components/CustomComponent/CustomComponent.h"
#include "Memory/CantMemory.h"

class GameObjectManager;
struct GameObjectDesc;
class ScriptingManager;

//To call init on each go's components, we need to pass these two
class AppRenderer;
class ResourceManager;

/**
 * @brief Class representing the most basic game unit on our engine
 * 
 */
class GameObject
{

public:
	friend class Factory;
	friend class GameObjectManager;
	friend class ScriptingManager;
	/**
	 * @brief Typedef of byteset (used for components)
	 * 
	 */
	typedef std::bitset< MAX_NUM_COMPONENTS > ComponentMask;

public:
	/**
	 * @brief Construct a new Game Object object
	 * 
	 * @param goMgr 
	 * @param prefabName 
	 */
	GameObject(GameObjectManager *goMgr, std::string prefabName);
	/**
	 * @brief Construct a new Game Object object, and sets a tag
	 * 
	 * @param goMgr 
	 * @param prefabName 
	 * @param tag 
	 */
	GameObject(GameObjectManager* goMgr, std::string prefabName, std::string tag);
	/**
	 * @brief Destroy the Game Object object
	 * 
	 */
	virtual ~GameObject();

	/**
	 * @brief Templatized. Returns true if GameObject owns a component of type T
	 * 
	 * @tparam T 
	 * @return true 
	 * @return false 
	 */
	template<typename T>
	bool HasComponent();
	
	/**
	 * @brief Templatized. Returns pointer to GameObject of type T if the GameObject owns it
	 * 
	 * @tparam T 
	 * @return true 
	 * @return false 
	 */
	template<typename T>
	T* GetComponent();
	
	/**
	 * @brief Templatized. Returns vector of components owned by the GameObject
	 * 
	 * @tparam T 
	 * @return true 
	 * @return false 
	 */
	template<typename T>
	std::vector<T*> GetAllComponents();
	
	/**
	 * @brief Templatized. Adds component of type T to GameObject (if its not already owned), and then return a T pointer to it
	 * 
	 * @tparam T 
	 * @return true 
	 * @return false 
	 */
	template<typename T>
	T* AddComponent();

	/**
	 * @brief Adds a custom (scripted) component to this GameObject
	 * 
	 * @param scriptName 
	 * @return CustomComponent* 
	 */
	CustomComponent *AddCustomComponent(const std::string& scriptName);//, ScriptingManager *luaMgr);
	/**
	 * @brief Get the Custom Component (if it exists)
	 * 
	 * @param scriptName 
	 * @return CustomComponent* 
	 */
	CustomComponent *GetCustomComponent(std::string scriptName);
	
	/**
	 * @brief Used to get a custom component from lua
	 * 
	 * @param scriptName 
	 * @return sol::table const& 
	 */
	sol::table const& LuaGetCustomComponent(std::string scriptName);
	/**
	 * @brief Used to add a custom component to a GameObject from lua
	 * 
	 * @param scriptName 
	 * @return sol::table 
	 */
	sol::table LuaAddCustomComponent(std::string scriptName);

	/**
	 * @brief Flags and queues a GameObject for removal next frame
	 * 
	 */
	void Destroy();
	/**
	 * @brief Getter for flag that states if the GameObject is marked for removal
	 * 
	 * @return true if its marked for removal
	 * @return false if it's not marked for removal
	 */
	bool Is_marked_for_remove() const;

	/**
	 * @brief Get the Component Mask
	 * 
	 * @return ComponentMask 
	 */
	ComponentMask GetComponentMask() const;
	/**
	 * @brief Gets the GameObject ID (also called from scripts)
	 * 
	 * @return size_t 
	 */
	size_t GetId() const;
	/**
	 * @brief Gets the Gameobject TAG (also called from scripts)
	 * 
	 * @return const std::string& 
	 */
	const std::string& GetTag() const;
	/**
	 * @brief Get the Prefab Name object
	 * 
	 * @return const std::string& 
	 */
	const std::string& GetPrefabName() const;
	/**
	 * @brief Gets the manager that owns this GameObject
	 * 
	 * @return GameObjectManager* 
	 */
	GameObjectManager *GetGOManager();


private:
	/**
	 * @brief Begin is called once, after an objects creation and instantiation. It calls Begin on all the GameObject's components.
	 when begin is called, all the other objects created this frame already have all of their components setup
	 * 
	 */
	void Begin();
	/**
	 * @brief  Init is called once, after an object's component have been overloaded. It calls Init on all the GameObject's components. When
	 Init is called, you can assume all the objects components exists
	 * @param pRenderer 
	 * @param pResMgr 
	 */
	void Init(AppRenderer *pRenderer, ResourceManager *pResMgr);

	/**
	 * @brief Instantiate methods, for creation of go's from script
	 * 
	 * @param goMgr 
	 * @return GameObject* 
	 */
	static GameObject *Instantiate(GameObjectManager *goMgr);
	/**
	 * @brief Instantiate methods, for creation of go's from script. Creates a clone of the Prefab described by prefabName parameter
	 * 
	 * @param goMgr 
	 * @return GameObject* 
	 */
	static GameObject *Instantiate(GameObjectManager *goMgr, 
		std::string const& prefabName);

//Variables
private:
	/**
	 * @brief Static variable. How many GameObjects have been created since beginning of execution
	 * 
	 */
	static int go_count;

	/**
	 * @brief Bitset used for knowing which components the GameObject owns
	 * 
	 */
	ComponentMask m_compMask;

	/**
	 * @brief Flag, if the GameObject is marked for removal
	 * 
	 */
	bool m_markedForRemoval;

private:
	/**
	 * @brief Id will be unique identifier, not optional, set by default 
	 * 
	 */
	size_t m_id;

	/**
	 * @brief Tag will be an optional identifier, set by the game designer in script or level editor 
	 * 
	 */
	std::string m_tag;
	/**
	 * @brief Name of the prefab on which this GameObject is based on
	 * 
	 */
	std::string m_prefabName;

	/**
	 * @brief Container for the ENGINE components
	 * 
	 */
	BaseComponent* m_components[MAX_NUM_COMPONENTS] = { 0 }; //128 bytes


	/**
	 * @brief Container for the SCRIPTED components
	 * 
	 */
	std::unordered_map<std::string, CustomComponent*> m_customComponents;
	
	/**
	 * @brief Pointer to the manager that handles this gameobj
	 * 
	 */
	GameObjectManager *m_gameObjectMgr;

	sol::table refHolder;

	RTTR_ENABLE();

#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
};



template<typename T>
bool GameObject::HasComponent()
{
	BaseComponent::ComponentId componentTypeId = T::static_type;
	return this->m_compMask[componentTypeId];
}


template<typename T>
T* GameObject::GetComponent()
{
	BaseComponent::ComponentId componentTypeId = T::static_type;

	if (!HasComponent<T>())
		return nullptr;

	return static_cast<T*>(m_components[componentTypeId]);
}


template<typename T>
std::vector<T*> GameObject::GetAllComponents()
{
	std::vector<T*> vec;

	//GET ALL CUSTOM COMP AND ADD TO VEC
	for (auto& node : m_customComponents)
		vec.push_back(node.second);

	return vec;
}


template <typename T>
T* GameObject::AddComponent()
{
	BaseComponent::ComponentId componentTypeId = T::static_type;
 	T* component = CantMemory::PoolResource<T>::Allocate(this);
	//DEBUG_LOG("Type: %s, Size: %d, Alignment %d, Pointer: %p \n, ", typeid(T).name(), sizeof(T), alignof(T), component);

	if (component)
	{
		this->m_compMask[componentTypeId] = 1;
		this->m_components[componentTypeId] = component;
		return component;
	}
	return nullptr;
}