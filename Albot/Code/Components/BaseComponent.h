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

#define MAX_NUM_COMPONENTS			32	//Max number of components (for bit string)


class BaseComponent 
{

//Friend classes
public:
	friend class ComponentFactory;
	typedef unsigned int ComponentId;

//Public interface
public:
	BaseComponent(GameObject *owner, ComponentId type)
	{
		this->m_owner = owner;
		this->m_type = type;
	}
	virtual ~BaseComponent() { }

	//Init should be called when instantiating the component
	virtual void Init() = 0;
	//Begin should be called once all the gameobject components have been created
	virtual void Begin() = 0;

	ComponentId GetType() const { return m_type; }


private:
	BaseComponent(BaseComponent& rhs);


protected:
	static ComponentId numberOfTypes;


protected:
	ComponentId m_type;
	GameObject *m_owner;
};