/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#pragma once


class ScriptingManager
{

public:
	ScriptingManager();
	~ScriptingManager();

	void Update();

	sol::table GetScriptDeepCopy(std::string scriptName);

private:
	sol::table LoadOrGetLuaScript(std::string scriptName);
	void ManageBindings();

public:
	sol::state luaState;

private:
	std::unordered_map<std::string, sol::table> m_scriptTableDic;
};

