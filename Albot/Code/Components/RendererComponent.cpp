/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RendererComponent.h"
unsigned const Renderer::static_type = BaseComponent::numberOfTypes++;


Renderer::Renderer(GameObject *owner) :
	BaseComponent(owner, Renderer::static_type)
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
}