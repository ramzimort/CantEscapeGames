/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include <iostream>

///Forward dec
class GameObject;


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
		this->type = type;
	}
	virtual ~BaseComponent() { }
	virtual void Init() = 0;
	ComponentId GetType() const { return type; }


private:
	BaseComponent(BaseComponent& rhs);


public:
	static ComponentId numberOfTypes;


protected:
	ComponentId type;
	GameObject *m_owner;
};