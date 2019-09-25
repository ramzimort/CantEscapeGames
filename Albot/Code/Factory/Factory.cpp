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
#include "Components/MeshesComponent.h"
#include "Components/LightComponent.h"



//TODO: albert stuff
#include "Managers/CameraManager.h"
#include "Graphics/Camera.h"
#include "Components/CameraComponent.h"
#include "Systems/FPSCameraSystem.h"
#include "Managers/ResourceManager.h"
extern CameraManager* gCameraManager;
extern ResourceManager* gResourceManager;


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
	//Wont be registered in any system				         ////
	GameObjectDesc desc4;							         ////
	desc4.tag = "FPSPlayer";						         ////
	desc4.componentSetup = [](GameObject *go)		         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();
		T->SetLocalPosition(0.f, 0.f, 0.f);////

		auto cameraComp = go->AddComponent<CameraComponent>();
		const CameraInfo* cameraInfo = gCameraManager->GetCameraInfo("Main");
		cameraComp->SetCamera(cameraInfo->m_camera);
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc4);


	Model* mitsubaSphereModel = gResourceManager->GetModel("mitsuba-sphere.obj", false);

	Material red_diffuse_purple_specular_desc = {};
	red_diffuse_purple_specular_desc.m_diffuse_color = Vector4(1.f, 0.f, 0.f, 1.f);
	red_diffuse_purple_specular_desc.m_specular_color = Vector4(1.f, 0.f, 1.f, 1.f);

	gResourceManager->StoreMaterial(red_diffuse_purple_specular_desc, "Red_Diffuse_Purple_Specular");

	Material* red_diffuse_purple_specular = gResourceManager->GetMaterial("Red_Diffuse_Purple_Specular");

	//Wont be registered in any system				         ////
	GameObjectDesc desc5 = {};							         ////
	desc5.tag = "mitsubaismybottom";						         ////
	desc5.initializeComponentSetup = [mitsubaSphereModel, red_diffuse_purple_specular](GameObject *go)	         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();
		T->SetLocalPosition(0.f, 0.f, 0.f);////

		go->AddComponent<TestComp>();

		auto rendererComp = go->AddComponent<RendererComponent>();
		rendererComp->SetMaterial(red_diffuse_purple_specular);
		
		auto meshesComp = go->AddComponent<MeshesComponent>();
		meshesComp->SetModel(mitsubaSphereModel);
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc5);


	Light directionalLight = {};
	directionalLight.m_cast_shadow = true;
	directionalLight.m_color = Vector3(1.f, 1.f, 1.f);
	directionalLight.m_intensity = 2.f;
	directionalLight.m_radius = 5.f;
	directionalLight.m_light_type = ELightType::DIRECTIONAL_LIGHT;

	GameObjectDesc desc6 = {};							         ////
	desc6.tag = "directionalLight";						         ////
	desc6.initializeComponentSetup = [directionalLight](GameObject *go)	         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();
		T->SetLocalPosition(0.f, 0.f, 0.f);////
		T->SetLocalRotation(60.0f, 30.0f, 0.0f);

		auto lightComp = go->AddComponent<LightComponent>();
		lightComp->SetLight(directionalLight);
		//Override code								         ////
	};												         ////
	goMgr->Queue_GameObject_Instantiation(&desc6);

	/////////////////////////////////////////////////////////////
}


Factory::~Factory()
{
}