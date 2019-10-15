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
#include "Events/Input/KeyEvent.h"
#include "Managers/EventManager.h"


CustomComponent::CustomComponent(GameObject *owner) :
	BaseComponent(owner, CustomComponent::static_type)
{
	//EventManager::Get()->SubscribeEvent<KeyEvent>(this, std::bind(&CustomComponent::AnyFunction, this, std::placeholders::_1));
}


CustomComponent::~CustomComponent()
{
}


void CustomComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	try
	{
		m_luaScriptTable["Init"](m_luaScriptTable);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName); //TODO - erase this
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
		DEBUG_LOG(errorName); //TODO - erase this
	}
}


sol::table& CustomComponent::getCustomCompLuaRef() 
{
	return m_luaScriptTable;
}


#include <Lua_53/lua.h>
void CustomComponent::ScriptSetup(StringId scriptPath, std::string const& name, ScriptingManager *luaMgr)
{
	//Setup lua script
	try
	{
		//Get a deep copy of the table so each table has their own state
		m_luaScriptTable = luaMgr->GetScriptDeepCopy(scriptPath);

		//Set the name
		this->m_name = name;

		// TODO - Bind some of the customComp methods to lua
		// Maybe pass the comp to the script as a "thisComp" (different from self, which is the table)
		///m_luaScriptTable["GetOwner"] = &CustomComponent::GetOwner;
		///m_luaScriptTable["GetName"] = &CustomComponent::GetName;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName); //TODO - erase this
	}
}

StringId const& CustomComponent::GetName() const
{
	return this->m_name;
}