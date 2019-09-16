/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "TestComponent.h"
unsigned const TestComp::static_type = BaseComponent::numberOfTypes++;


TestComp::TestComp(GameObject *owner) : 
	BaseComponent(owner, TestComp::static_type) 
{
}

TestComp::~TestComp() 
{
}

void TestComp::Init() 
{
}