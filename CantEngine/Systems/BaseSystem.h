/**
 * @file BaseSystem.h
 * @author your name (you@domain.com)
 * @brief Base class for Systems
 * @date 2019-11-21
 */

#pragma once

///Includes
#include "../Components/BaseComponent.h"


/**
 * @brief Base System component node. Will be used to hold pointers of 
 components belonging to each GameObject suscribed to this system
 * 
 */
struct BaseSystemCompNode
{
	size_t m_goID;
};


/**
 * @brief Base class for Systems
 * 
 */
class BaseSystem 
{

//Friend classes
public:
	friend class Factory;
	friend class SystemManager;

//Public interface
public:
	/**
	 * @brief Constructor for base class. Called by inheriting classes
	 * 
	 */
	BaseSystem() { }
	/**
	 * @brief Destroy the Base System object
	 * 
	 */
	virtual ~BaseSystem();
	
	/**
	 * @brief Early update called once before main Update. When called, you have access to every 
	 GameObject registered and can manage interaction between them
	 * 
	 * @param dt 
	 */
	virtual void EarlyUpdate(float dt) {}
	/**
	 * @brief Iterates through all GameObjects registered and calls update
	 * 
	 * @param dt 
	 */
	void UpdateAllNodes(float dt);
	/**
	 * @brief Late update called once after main Update. When called, you have access to every 
	 GameObject registered and can manage interaction between them
	 * 
	 * @param dt 
	 */
	virtual void LateUpdate(float dt) {}
	/**
	 * @brief Regular Update call. Called once per registered GameObject by UpdateAllNodes
	 * 
	 * @param dt 
	 * @param compNode 
	 */
	virtual void Update(float dt,
		BaseSystemCompNode *compNode) {}

	/**
	 * @brief Regular Draw call. Called once per registered GameObject by DrawAllNodes
	 * 
	 */
	virtual void Draw(float dt,
		BaseSystemCompNode *compNode);
	/**
	 * @brief Iterates through all registered GameObjects and calls Draw
	 * 
	 * @param dt 
	 */
	void DrawAllNodes(float dt);

protected:
	/**
	 * @brief Used to setup the system, so for every registered GameObject node, we have pointer to the components that are updated on this system
	 * 
	 * @tparam T 
	 */
	template <typename T>
	void Push_required_comp();

private:
	/**
	 * @brief Private copy constructor to avoid making copies
	 * 
	 * @param rhs 
	 */
	BaseSystem(BaseSystem const& rhs);

	/**
	 * @brief Returns true if GameObject GO can be registered in this system
	 * 
	 * @param go 
	 * @return true 
	 * @return false 
	 */
	bool Can_Register_GameObject(GameObject *go);
	/**
	 * @brief Registers GameObject GO in this system
	 * 
	 * @param go 
	 */
	virtual void Register_GameObject(GameObject *go) = 0;
	/**
	 * @brief Unregisters GameObject GO from this system
	 * 
	 * @param go_id 
	 */
	virtual void Unregister_GameObject(size_t go_id);

protected:
	/**
	 * @brief Static member. Number of system types
	 * 
	 */
	static unsigned int numberOfTypes;

protected:
	/**
	 * @brief Byteset used to mask against GameObjects component mask and check if they can register
	 * 
	 */
	std::bitset<MAX_NUM_COMPONENTS> m_requiredCompMask;
	/**
	 * @brief Hash map holding the nodes of registered GameObjects components
	 * 
	 */
	std::unordered_map<size_t, BaseSystemCompNode*> m_ObjComponentsMap;
};



template <typename T>
void BaseSystem::Push_required_comp() 
{
	unsigned componentId = T::static_type;
	this->m_requiredCompMask[componentId] = 1;
}