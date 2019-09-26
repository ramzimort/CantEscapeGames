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
class Rigidbody;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct RigidbodyCompNode : BaseSystemCompNode
{
	TransformComponent *n_transform;
	Rigidbody *n_rigidbody;

	//Ctor
	RigidbodyCompNode(TransformComponent *transform,
		Rigidbody *rigidbody) : n_transform(transform),
		n_rigidbody(rigidbody)
	{}
};


class RigidbodySystem : public BaseSystem
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	RigidbodySystem();
	virtual ~RigidbodySystem() {}

	virtual void Register_GameObject(GameObject *go) override;

	virtual void LateUpdate(float dt) override;

protected:
	//protected vars

public:
	//To compare when using templates
	static unsigned int const static_type;
};