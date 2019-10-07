/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "../BaseComponent.h"

class GameObjectManager;
class ScriptingManager;


class CustomComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class CustomSystem;
	friend class ScriptingManager;

//Public interface
public:
	CustomComponent(GameObject *owner);
	virtual ~CustomComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;

	//TODO - Check if leaving them or not
	std::string const& GetName() const;

	//Scripts methods
	sol::table& getCustomCompLuaRef();
	void ScriptSetup(std::string scriptName, 
		ScriptingManager *luaMgr);

	template<typename T>
	void Override(std::string member, T value);

public:
	//Unique class identifier
	static ComponentId const static_type; //TODO

private:
	//It will need a name identifier
	std::string m_name;

	//It needs a reference or pointer to the lua state
	sol::table m_luaScriptTable;

};


template<typename T>
void CustomComponent::Override(std::string member, T value) 
{
	//Overriding before the script has been set
	if (m_luaScriptTable == sol::lua_nil) 
		return;

	try 
	{
		m_luaScriptTable[member] = value;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
	}
}