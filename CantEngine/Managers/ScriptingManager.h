/**
 * @file ScriptingManager.h
 * @author Jose Rosenbluth
 * @date 12/4/2019
 * @brief Class that manages the scripting and the bindings from cpp to lua
 */


#pragma once

#include "Helper/Hash.h"

class ResourceManager;
class AppRenderer;

/**
 * @brief Class that manages the scripting and the bindings from cpp to lua
 * 
 */
class ScriptingManager
{

public:
	/**
	 * @brief Construct a new Scripting Manager object
	 * 
	 * @param pResourcemanager 
	 * @param pAppRenderer 
	 */
	ScriptingManager(ResourceManager* pResourcemanager, AppRenderer* pAppRenderer);
	/**
	 * @brief Destroy the Scripting Manager object
	 * 
	 */
	~ScriptingManager();

	/**
	 * @brief Update call for the scripting manager
	 * 
	 */
	void Update();

	/**
	 * @brief Given a script's name, it will search for the lua table reference, 
	 deep copy it, and return it. This way, several objects can have the same 
	 scripted component while also having their own state
	 * 
	 * @param scriptName 
	 * @return sol::table 
	 */
	sol::table GetScriptDeepCopy(StringId scriptName);

private:
	/**
	 * @brief Sets up all the cpp to lua bindings necessary to use the engine from the lua scripts
	 * 
	 */
	void ManageBindings();
	//const std::wstring& GetString(const std::string&);

public:
	/**
	 * @brief Main Lua state. All the scripts in the game are ran from the same state
	 * 
	 */
	sol::state luaState;

private:
	/**
	 * @brief Pointer to the resource manager
	 * 
	 */
	ResourceManager* m_pResourceManager;
	/**
	 * @brief Pointer to the graphic's manager
	 * 
	 */
	AppRenderer* m_pAppRenderer;
	/**
	 * @brief Hash table that holds pairs of string, table reference
	 * 
	 */
	std::unordered_map<std::string, sol::table> m_scriptTableDic;
};

