/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#pragma once


#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"


class ScriptingManager
{

public:
	ScriptingManager();
	~ScriptingManager();

	void Update();

private:
	void ManageBindings();

public:
	sol::state luaState;
};

