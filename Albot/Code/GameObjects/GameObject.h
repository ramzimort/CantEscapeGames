/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

#include "Components/BaseComponent.h"


class GameObjectManager;


class GameObject
{

public:
	friend class Factory;
	typedef std::bitset< MAX_NUM_COMPONENTS > ComponentMask;

public:
	GameObject(GameObjectManager *goMgr);
	GameObject(GameObjectManager *goMgr, std::string tag);
	virtual ~GameObject();

	//Component methods
	template<typename T>
	bool HasComponent();
	template<typename T>
	T* GetComponent();
	template<typename T>
	T* AddComponent();

	//Remove functions
	void Destroy();
	bool Is_marked_for_remove() const;

	//Getters
	ComponentMask GetComponentMask() const;
	size_t GetId() const;
	std::string GetTag() const;

//Variables
private:
	static int go_count;

	ComponentMask m_compMask;
	bool m_markedForRemoval;


private:
	//Id will be unique identifier, not optional, set by default.
	size_t m_id;

	//Tag will be an optional identifier, set by us in the json file
	std::string m_tag;

	BaseComponent* m_components[MAX_NUM_COMPONENTS] = { 0 }; //128 bytes
	GameObjectManager *m_gameObjectMgr;
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


template <typename T>
T* GameObject::AddComponent()
{
	BaseComponent::ComponentId componentTypeId = T::static_type;
	T* component = new T(this);

	if (component)
	{
		this->m_compMask[componentTypeId] = 1;
		this->m_components[componentTypeId] = component;
		return component;
	}
	return nullptr;
}

