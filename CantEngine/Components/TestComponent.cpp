/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "TestComponent.h"
#include "Graphics/D3D11_Renderer.h"
unsigned const TestComp::static_type = BaseComponent::numberOfTypes++;

#include "Managers/GameObjectManager.h"

RTTR_REGISTRATION
{
}



TestComp::TestComp(GameObject *owner) : 
	BaseComponent(owner, TestComp::static_type) 
{
}

TestComp::~TestComp() 
{
}

void TestComp::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
}

void TestComp::Begin(GameObjectManager *goMgr) 
{
}