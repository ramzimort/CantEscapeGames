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
	 * @brief Update is called once
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

	virtual void Update(float dt,
		BaseSystemCompNode *compNode) {}

	virtual void Draw(float dt,
		BaseSystemCompNode *compNode);

	void DrawAllNodes(float dt);

protected:
	template <typename T>
	void Push_required_comp();

private:
	BaseSystem(BaseSystem const& rhs);

	//This methods should be called by the systemMgr
	bool Can_Register_GameObject(GameObject *go);
	virtual void Register_GameObject(GameObject *go) = 0;
	virtual void Unregister_GameObject(size_t go_id);

protected:
	static unsigned int numberOfTypes;

protected:
	//Identifier of required components
	std::bitset<MAX_NUM_COMPONENTS> m_requiredCompMask;

	//Map of go_id to node holding all its components
	std::unordered_map<size_t, BaseSystemCompNode*> m_ObjComponentsMap;
};



template <typename T>
void BaseSystem::Push_required_comp() 
{
	unsigned componentId = T::static_type;
	this->m_requiredCompMask[componentId] = 1;
}