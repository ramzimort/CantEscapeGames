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
#include "Graphics/ModelLoader.h"
#include "Graphics/AppRenderer.h"
extern CameraManager* gCameraManager;
extern ResourceManager* gResourceManager;
extern AppRenderer* gAppRenderer;


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


	gResourceManager->LoadModel("mitsuba-sphere.obj", false);

	Model* mitsubaSphereModel = gResourceManager->GetModel(SID("mitsuba-sphere.obj"));

	Material* red_diffuse_purple_specular = new Material();
	red_diffuse_purple_specular->m_diffuse_color = Vector4(1.f, 0.f, 0.f, 1.f);
	red_diffuse_purple_specular->m_specular_color = Vector4(1.f, 0.f, 1.f, 1.f);

	//gResourceManager->GetMaterial(red_diffuse_purple_specular_desc, "Red_Diffuse_Purple_Specular");

	//Material* red_diffuse_purple_specular = gResourceManager->GetMaterial("Red_Diffuse_Purple_Specular");

	//Wont be registered in any system				         ////
	GameObjectDesc desc5 = {};							         ////
	desc5.tag = "mitsubaismybottom";						         ////
	desc5.initializeComponentSetup = [mitsubaSphereModel, red_diffuse_purple_specular](GameObject *go)	         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();
		T->SetLocalPosition(0.f, 0.f, 0.f);////

		//go->AddComponent<TestComp>();

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




	Material* red_brick_material = new Material();
	red_brick_material->m_diffuse_color = Vector4(1.f, 1.f, 1.f, 1.f);
	red_brick_material->m_specular_color = Vector4(1.f, 1.f, 1.f, 1.f);

	gResourceManager->LoadTexture(Constant::TexturesDir + "redbrick.jpg");
	gResourceManager->LoadTexture(Constant::TexturesDir + "redbrick_normal.jpg");
	gResourceManager->LoadTexture(Constant::TexturesDir + "redbrick_height.jpg");

	red_brick_material->m_diffuse_texture = gResourceManager->GetTexture(SID(Constant::TexturesDir + "redbrick.jpg"));
	red_brick_material->m_normal_texture = gResourceManager->GetTexture(SID(Constant::TexturesDir + "redbrick_normal.jpg"));
	red_brick_material->m_height_texture = gResourceManager->GetTexture(SID(Constant::TexturesDir + "redbrick_height.jpg"));

	Model* plane_model = new Model();

	plane_model->m_vertices.push_back(VertexData(Vector3(-1.f, 1.f, 0.1f), Vector3(0.f, 0.f, 1.f), Vector2(0.0f, 1.0f)));
	plane_model->m_vertices.push_back(VertexData(Vector3(-1.f, -1.f, 0.1f), Vector3(0.f, 0.f, 1.f), Vector2(0.0f, 0.0f)));
	plane_model->m_vertices.push_back(VertexData(Vector3(1.f, -1.f, 0.1f), Vector3(0.f, 0.f, 1.f), Vector2(1.0f, 0.0f)));
	plane_model->m_vertices.push_back(VertexData(Vector3(1.f, 1.f, 0.1f), Vector3(0.f, 0.f, 1.f), Vector2(1.0f, 1.0f)));

	plane_model->m_triangle_indices.push_back(Model::Triangle{ 0, 1, 3 });
	plane_model->m_triangle_indices.push_back(Model::Triangle{ 1, 2, 3 });
	plane_model->InitBuffer(gAppRenderer->GetDXRenderer());

	ModelLoader::CalculateModelTangents(*plane_model);


	GameObjectDesc desc7 = {};
	desc7.tag = "redbrick_plane";
	desc7.initializeComponentSetup = [plane_model, red_brick_material](GameObject *go)	         ////
	{												         ////
		auto *T = go->AddComponent<Transform>();
		T->SetLocalPosition(0.f, -1.f, 0.f);////
		T->Rotate(90.f, 0.f, 0.f);
		T->Scale(50.f, 50.f, 1.f);

		//go->AddComponent<TestComp>();

		auto rendererComp = go->AddComponent<RendererComponent>();
		rendererComp->SetMaterial(red_brick_material);

		auto meshesComp = go->AddComponent<MeshesComponent>();
		meshesComp->SetModel(plane_model);
		//Override code								         ////
	};												         ////
	//goMgr->Queue_GameObject_Instantiation(&desc7);

	/////////////////////////////////////////////////////////////
}


Factory::~Factory()
{
}