/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#pragma once

#include "Helper/Hash.h"

class ResourceManager;
class AppRenderer;

class ScriptingManager
{

public:
	ScriptingManager(ResourceManager* pResourcemanager, AppRenderer* pAppRenderer);
	~ScriptingManager();

	void Update();

	sol::table GetScriptDeepCopy(StringId scriptName);

private:
	void ManageBindings();
	//const std::wstring& GetString(const std::string&);

public:
	sol::state luaState;

private:
	ResourceManager* m_pResourceManager;
	AppRenderer* m_pAppRenderer;
	std::unordered_map<std::string, sol::table> m_scriptTableDic;
};

