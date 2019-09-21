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


class TestComp : public BaseComponent
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	TestComp(GameObject *owner);
	virtual ~TestComp();

	virtual void Init() override;
	virtual void Begin() override;

public:
	//Unique class identifier
	static ComponentId const static_type;

	//Multicast testing
	Multicast<void(float)> onWhateverFires;

};