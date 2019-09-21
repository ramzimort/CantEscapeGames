/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "GameObject.h"
#include "../Managers/GameObjectManager.h"


// Initialize static member of class
int GameObject::go_count = 0;


GameObject::GameObject(GameObjectManager *goMgr) :
	m_gameObjectMgr(goMgr), m_id(go_count++), m_compMask(0),
	m_markedForRemoval(false)
{
}

GameObject::GameObject(GameObjectManager *goMgr, std::string tag) :
	m_gameObjectMgr(goMgr), m_id(go_count++), m_compMask(0), 
	m_markedForRemoval(false), m_tag(tag)
{
}

GameObject::~GameObject()
{
	for (int i = 0; i < MAX_NUM_COMPONENTS; ++i) 
	{
		delete m_components[i];
	}
}

size_t GameObject::GetId() const
{
	return this->m_id;
}

GameObject::ComponentMask GameObject::GetComponentMask() const
{
	return m_compMask;
}

std::string GameObject::GetTag() const
{
	return this->m_tag;
}

void GameObject::Destroy()
{
	this->m_markedForRemoval = true;

	//Stuff that must happen when a gameobject is marked for remove

	// 1- Communicate to the GO MGR so the go is queued for deletion
	// TODO - Change for event call
	m_gameObjectMgr->Queue_GameObject_Destruction(this->GetId());
}

bool GameObject::Is_marked_for_remove() const
{
	return m_markedForRemoval;
}