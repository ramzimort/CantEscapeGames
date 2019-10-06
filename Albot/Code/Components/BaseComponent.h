/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes

///Forward dec
class GameObject;
class ResourceManager;
class DXRenderer;
#define MAX_NUM_COMPONENTS			32	//Max number of components (for bit string)

class GameObjectManager;


class BaseComponent 
{

//Friend classes
public:
	friend class Factory;
	friend class GameObject;
	typedef unsigned int ComponentId;

//Public interface
public:
	BaseComponent(GameObject *owner, ComponentId type)
	{
		this->m_owner = owner;
		this->m_type = type;
	}
	virtual ~BaseComponent() { }

	ComponentId GetType() const { return m_type; }

	GameObject* GetOwner() const { return m_owner; }

private:
	BaseComponent(BaseComponent& rhs);

	//Init should be called when instantiating the component
	virtual void Init(ResourceManager* res, DXRenderer* dxrenderer) = 0;
	//Begin should be called once all the gameobject components have been created
	virtual void Begin(GameObjectManager *goMgr) = 0;


protected:
	static ComponentId numberOfTypes;


protected:
	ComponentId m_type;
	GameObject *m_owner;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};