/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "../BaseSystem.h"


class CustomComponent;


//CustomSystem will only update CustomComponent
//One gameObj can have multiple CustomComponents, so that has to be accounted for in the node
struct CustomCompNode : BaseSystemCompNode
{
	//In this case, we can have an arbitrary 
	//number of custom components
	std::vector<CustomComponent*> n_components;

	//Constructor
	CustomCompNode(std::vector<CustomComponent*> const& compList)
	{
		for (CustomComponent *comp : compList)
		{
			n_components.push_back(comp);
		}
	}
};


class CustomSystem : public BaseSystem
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	CustomSystem();
	virtual ~CustomSystem() {}

	virtual void Register_GameObject(GameObject *go);
	virtual void Update(float dt, BaseSystemCompNode *compNode);

public:
	//To compare when using templates
	static unsigned int const static_type;
};