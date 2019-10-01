/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "TestComponent.h"
unsigned const TestComp::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
}

#include "Managers/ScriptingManager.h"
extern ScriptingManager *luaMgr;



TestComp::TestComp(GameObject *owner) : 
	BaseComponent(owner, TestComp::static_type) 
{
	//TODO - temporary
	this->luaState = &(luaMgr->luaState);
}

TestComp::~TestComp() 
{
}

void TestComp::Init(ResourceManager* resMgr)
{
	//CPP STUFF


	//Get the Lua version and call it
	sol::protected_function InitLua = (*luaState)["Init"];
	InitLua();
}

void TestComp::Begin()
{
	//CPP STUFF


	//Get the Lua version and call it
	sol::protected_function BeginLua = (*luaState)["Begin"];
	BeginLua();
}