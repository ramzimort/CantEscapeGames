/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "GameObject.h"
#include <iostream>


// Initialize static member of class
int GameObject::go_count = 0;


GameObject::GameObject() : 
	id(go_count++), marked_for_remove(false)
{
}

GameObject::GameObject(std::string tag) :
	id(go_count++), marked_for_remove(false), tag(tag)
{
}

GameObject::~GameObject()
{
	//for (auto& node : components) 
	//{
	//	delete node.second;
	//}
	//components.clear();
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
}

bool GameObject::Is_marked_for_remove() const
{
	return marked_for_remove;
}