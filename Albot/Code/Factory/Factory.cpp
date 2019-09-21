/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "Factory.h"
#include "GameObjects/GameObject.h"
#include "Managers/SystemManager.h"
#include "Managers/GameObjectManager.h"

// ALL COMPONENTS
#include "Components/TransformComponent.h"
#include "Components/RendererComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Components/TestComponent.h"


Factory::Factory(std::string path, GameObjectManager *goMgr, SystemManager *sysMgr)
{
	//If either of these is nullptr, we have to stop
	if (!goMgr || !sysMgr) 
	{
		//TODO - Call debug stuff here for error
		return;
	}


	/////////////////////////////////////////////////////////////
	//   LATER, THIS PART HAS TO BE DONE USING THE PATH      ////
	/////////////////////////////////////////////////////////////
	//   EXAMPLE OF HOW TO ADD GAMEOBJECTS                   ////
                                                             ////
													         ////
	//Will register in rendering system				         ////
	GameObjectDesc desc1;							         ////
	desc1.tag = "monoRojo";							         ////
	desc1.componentSetup = [](GameObject *go)		         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();	         ////
		//Override code								         ////
													         ////
		auto *R = go->AddComponent<RendererComponent>();     ////
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc1);	         ////
													         ////
													         ////
													         ////
	//Will register in rigidbody system				         ////
	GameObjectDesc desc2;							         ////
	desc2.tag = "monoAzul";							         ////
	desc2.componentSetup = [](GameObject *go)		         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();	         ////
		//Override code								         ////
		                                                     ////
		auto *R = go->AddComponent<Rigidbody>();	         ////
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc2);	         ////
													         ////
													         ////
													         ////
	//Wont be registered in any system				         ////
	GameObjectDesc desc3;							         ////
	desc3.tag = "monoVerde";						         ////
	desc3.componentSetup = [](GameObject *go)		         ////
	{												         ////
		auto *T = go->AddComponent<TestComp>();		         ////
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc3);	         ////
													         ////
	/////////////////////////////////////////////////////////////
}


Factory::~Factory()
{
}