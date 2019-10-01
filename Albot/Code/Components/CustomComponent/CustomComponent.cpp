/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "CustomComponent.h"
unsigned const CustomComponent::static_type = static_cast<unsigned>(-1);

#include "GameObjects/GameObject.h"

#include "Managers/ScriptingManager.h"
extern ScriptingManager *luaMgr;



CustomComponent::CustomComponent(GameObject *owner) :
	BaseComponent(owner, CustomComponent::static_type)
{
	//TODO - temporary
	this->luaState = &(luaMgr->luaState);
}


CustomComponent::~CustomComponent()
{
}


void CustomComponent::Init(ResourceManager* resMgr)
{
	try
	{
		sol::table self = (*luaState)[this->name];
		self["Init"](self);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}

void CustomComponent::Begin()
{
	try
	{
		sol::table self = (*luaState)[this->name];
		self["Begin"](self);// , this->GetOwner());
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}



#include <Lua_53/lua.h>
void CustomComponent::ScriptSetup(std::string scriptName)
{
	//Setup lua script
	try
	{
		//Load the script (TODO - find out if there is a better call)
		///state_call_result = 
		luaState->script_file("Code/Scripts/" + scriptName + ".lua");
		///
		///if (state_call_result.valid()) 
		///{
		///	state_table = state_call_result;
		///}

		/// //--------------
		/// // create a table to act as the environment
		/// sol::table environment = luaState->create_table();
		/// 
		/// // load the script into a function
		/// sol::load_result script = luaState->load_file("Code/Scripts/" + scriptName + ".lua");
		/// 
		/// // use the __index metamethod to give the env read access to 
		/// // global environment (to give the env access to loaded Lua std libs)
		/// environment[sol::metatable_key] = luaState->create_table_with
		/// (
		/// 	sol::meta_function::index, luaState->globals()
		/// );
		/// 
		/// // push the environment table onto the stack to use it as the parameter for setfenv
		/// environment.push();
		/// 
		/// // call setfenv with the stack index of the loaded function
		/// lua_setfenv(*luaState, script.stack_index());
		/// 
		/// // call the loaded function now that its env is setup
		/// script();
		/// 
		/// // get the value of the variable from the environment
		/// std::string str1 = environment["var"]; // str1 will equal "script1"
		/// //--------------


		//Set the name
		this->name = scriptName;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}

//TODO - dont push
GameObject *CustomComponent::GetOwner() const
{
	return this->m_owner;
}

//TODO - dont push
std::string const& CustomComponent::GetName() const
{
	return this->name;
}