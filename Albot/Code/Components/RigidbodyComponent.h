/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"
#include "Physics/Geometry/Aabb.h"


class RigidbodyComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	RigidbodyComponent(GameObject *owner);
	virtual ~RigidbodyComponent();

	virtual void Init() override;
	virtual void Begin() override;

public:
	//Unique class identifier
	static ComponentId const static_type;

	Aabb m_aabb;
	unsigned int dynamicAabbTreeKey;
};
