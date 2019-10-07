/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "CustomComponent.h"
unsigned const CustomComponent::static_type = static_cast<unsigned>(-1);

#include "GameObjects/GameObject.h"
#include "Managers/GameObjectManager.h"
#include "Managers/ScriptingManager.h"



CustomComponent::CustomComponent(GameObject *owner) :
	BaseComponent(owner, CustomComponent::static_type)
{
}


CustomComponent::~CustomComponent()
{
}


void CustomComponent::Init(ResourceManager* resMgr)
{
	try
	{
		m_luaScriptTable["Init"](m_luaScriptTable);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}

void CustomComponent::Begin(GameObjectManager *goMgr)
{
	try
	{
		m_luaScriptTable["Begin"](m_luaScriptTable, GetOwner(), goMgr);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}


sol::table& CustomComponent::getCustomCompLuaRef() 
{
	return m_luaScriptTable;
}


#include <Lua_53/lua.h>
void CustomComponent::ScriptSetup(std::string scriptName, ScriptingManager *luaMgr)
{
	//Setup lua script
	try
	{
		//Get a deep copy of the table so each table has their own state
		m_luaScriptTable = luaMgr->GetScriptDeepCopy(scriptName);

		//Set the name
		this->m_name = scriptName;

		// TODO - Bind some of the customComp methods to lua
		// Maybe pass the comp to the script as a "thisComp" (different from self, which is the table)
		///m_luaScriptTable["GetOwner"] = &CustomComponent::GetOwner;
		///m_luaScriptTable["GetName"] = &CustomComponent::GetName;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}

//TODO - dont push
std::string const& CustomComponent::GetName() const
{
	return this->m_name;
}