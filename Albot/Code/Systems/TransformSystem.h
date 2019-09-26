/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"

class TransformComponent;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct TransformCompNode : BaseSystemCompNode
{
	TransformComponent *n_transform;

	//Ctor
	TransformCompNode(TransformComponent *transform) :
		n_transform(transform)
	{}
};


class TransformSystem : public BaseSystem
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	TransformSystem();
	virtual ~TransformSystem() {}

	virtual void Register_GameObject(GameObject *go);
	virtual void Update(float dt, BaseSystemCompNode *compNode);

public:
	//To compare when using templates
	static unsigned int const static_type;
};