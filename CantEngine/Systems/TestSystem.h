/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"

class TestComp;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct TestCompCompNode : BaseSystemCompNode
{
	TestComp *n_testComp;

	//Ctor
	TestCompCompNode(TestComp *test) :
		n_testComp(test)
	{}
};


class TestSystem : public BaseSystem
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	TestSystem();
	virtual ~TestSystem() {}

	virtual void Register_GameObject(GameObject *go);
	virtual void Update(float dt, BaseSystemCompNode *compNode);

protected:
	void printFloat(float a);

public:
	//To compare when using templates
	static unsigned int const static_type;
};