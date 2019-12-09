/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"

#include "../Events/Multicast.h"

//Testing it works
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

class GameObjectManager;


class TestComp : public BaseComponent
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	TestComp(GameObject *owner);
	virtual ~TestComp();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;

public:
	//Unique class identifier
	static ComponentId const static_type;

	//Multicast testing
	Multicast<void(std::string, float)> onWhateverFires;

};