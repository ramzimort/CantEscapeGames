/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
///#include <bitset>
///#include <unordered_map>
#include "../Components/BaseComponent.h"


//Will be used to hold the node of components 
//belonging to each GameObject suscribed
struct BaseSystemCompNode
{
	size_t m_goID;
};



class BaseSystem 
{

//Friend classes
public:
	friend class Factory;
	friend class SystemManager;

//Public interface
public:
	BaseSystem() { }
	virtual ~BaseSystem();
	
	virtual void EarlyUpdate(float dt) {}
	void UpdateAllNodes(float dt);
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