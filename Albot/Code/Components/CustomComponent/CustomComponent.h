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


//Testing it works
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"


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

	virtual void Init(ResourceManager* resMgr) override;
	virtual void Begin() override;

	//TODO - Check if leaving them or not
	GameObject *GetOwner() const;
	std::string const& GetName() const;

	void ScriptSetup(std::string scriptName);

public:
	//Unique class identifier
	static ComponentId const static_type; //TODO

private:
	//It will need a name identifier
	std::string name;

	//It needs a reference or pointer to the lua state
	sol::state *luaState;

};