/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "GameObject.h"

//TODO - Remove when replacing for event call
#include "Managers/GameObjectManager.h"
extern GameObjectManager *goMgr;


// Initialize static member of class
int GameObject::go_count = 0;


GameObject::GameObject() : 
	id(go_count++), comp_mask(0), 
	marked_for_remove(false)
{
}

GameObject::GameObject(std::string tag) :
	id(go_count++), comp_mask(0), 
	marked_for_remove(false), tag(tag)
{
}

GameObject::~GameObject()
{
	for (int i = 0; i < MAX_NUM_COMPONENTS; ++i) 
	{
		delete components[i];
	}
}

size_t GameObject::GetId() const
{
	return this->id;
}

GameObject::ComponentMask GameObject::GetComponentMask() const
{
	return comp_mask;
}

std::string GameObject::GetTag() const
{
	return this->tag;
}

void GameObject::Destroy()
{
	this->marked_for_remove = true;

	//Stuff that must happen when a gameobject is marked for remove

	// 1- Communicate to the GO MGR so the go is queued for deletion
	// TODO - Change for event call
	goMgr->Queue_GameObject_Destruction(this->GetId());
}

bool GameObject::Is_marked_for_remove() const
{
	return marked_for_remove;
}