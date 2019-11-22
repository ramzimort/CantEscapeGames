/**
 * @file CustomComponent.h
 * @author Jose Rosenbluth
 * @brief Class that represents the CPP class behind a scripted component
 * @date 2019-11-21
 */

#pragma once

///INCLUDES
#include "Components/BaseComponent.h"
#include "Helper/Hash.h"

class GameObjectManager;
class ScriptingManager;
class KeyEvent;

/**
 * @brief Class that represents the CPP class behind a scripted component
 * 
 */
class CustomComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class CustomSystem;
	friend class ScriptingManager;

//Public interface
public:
	/**
	 * @brief Construct a new Custom Component object
	 * 
	 * @param owner 
	 */
	CustomComponent(GameObject *owner);
	/**
	 * @brief Destroy the Custom Component object
	 * 
	 */
	virtual ~CustomComponent();

	/**
	 * @brief Will do nothing on CPP other than call Init on the script
	 * 
	 * @param resMgr 
	 * @param dxrenderer 
	 */
	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	/**
	 * @brief Will do nothing on CPP other than call Begin on the script
	 * 
	 * @param goMgr 
	 */
	virtual void Begin(GameObjectManager *goMgr) override;

	/**
	 * @brief Get the Name of this scripted component (same name as script)
	 * 
	 * @return StringId const& 
	 */
	StringId const& GetName() const;

	/**
	 * @brief Returns the lua table reference that corresponds to this Component's script
	 * 
	 * @return sol::table& 
	 */
	sol::table& getCustomCompLuaRef();
	/**
	 * @brief Sets the script up, by deep copying and getting the table reference from the ScriptingManager
	 * 
	 * @param scriptPath 
	 * @param name 
	 * @param luaMgr 
	 */
	void ScriptSetup(StringId scriptPath, std::string const& name, ScriptingManager *luaMgr);

	/**
	 * @brief Overrides a value on the script. Templatized since it can override different data types (as long as they exist on LUA)
	 * 
	 * @tparam T 
	 * @param member 
	 * @param value 
	 */
	template<typename T>
	void Override(std::string member, T value);

	//Even on key down
	//void AnyFunction(const KeyEvent *keyEvent);

public:
	/**
	 * @brief Unique type identifier
	 * 
	 */
	static ComponentId const static_type; //TODO

private:
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
	/**
	 * @brief Script's name
	 * 
	 */
	StringId m_name;

	/**
	 * @brief Lua table that references this object's script
	 * 
	 */
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