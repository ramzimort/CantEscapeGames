/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RigidbodyComponent.h"
unsigned const Rigidbody::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<Rigidbody>("Rigidbody")
		.method("Init", &Rigidbody::Init)
		.method("Begin", &Rigidbody::Begin);
}

Rigidbody::Rigidbody(GameObject *owner) :
	BaseComponent(owner, Rigidbody::static_type)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Init()
{
}

void Rigidbody::Begin()
{
}