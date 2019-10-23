
#ifdef DEVELOPER
#include "DebugManager.h"
#include "Managers/GameObjectManager.h"
#include "Components/AllComponentHeaders.h"
#include "GameObjects/GameObject.h"
#include "Managers/EventManager.h"
#include "Graphics/Camera.h"
#include "Directory/Directory.h"
#include "Factory/Factory.h"
#include "Reflection/Helpers.h"
#include "Reflection/Serialization.h"

namespace CantDebug
{
	using namespace std;
	template<typename T>
	void WriteComponentOverride(T* component, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	void ReadOverrides(GameObject* go, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	vector<Info> InitializeList(const std::string& name)
	{
		auto list = CantDirectory::GetAllObjects(name);
		vector<Info> result;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			Info info;
			info.FullPath = name + *it;
			info.Name = it->substr(1, it->length()-1);
			info.Pressed = false;
			info._pressed = false;
			result.push_back(info);
		}
		return result;
	}

	DebugManager::DebugManager(AppRenderer* pAppRenderer, ResourceManager* pResourceManager, StateManager* pStateManager) :
		m_pAppRenderer(pAppRenderer), m_pResourceManager(pResourceManager), m_pStateManager(pStateManager), m_pGameState(nullptr)
	{
		EventManager::Get()->SubscribeEvent<GameObjectCreated>(this, std::bind(&DebugManager::RegisterObject, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<GameObjectDestroyed>(this, std::bind(&DebugManager::UnregisterObject, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<MouseClickEvent>(this, std::bind(&DebugManager::OnClick, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<MouseMotionEvent>(this, std::bind(&DebugManager::OnMotion, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<WindowSizeEvent>(this, std::bind(&DebugManager::OnScreenResize, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<KeyEvent>(this, std::bind(&DebugManager::OnKey, this, std::placeholders::_1));

		Initialize();
	}

	DebugManager::~DebugManager() { }

	void DebugManager::Initialize()
	{
		m_resources["Assets\\Textures\\"] = InitializeList("Assets\\Textures");
		m_resources["Assets\\Models\\"] = InitializeList("Assets\\Models");
		m_resources["Assets\\Materials\\"] = InitializeList("Assets\\Materials");
		m_resources["Scripts\\"] = InitializeList("Scripts");
		m_resources["Assets\\Prefabs\\"] = InitializeList("Assets\\Prefabs");
		m_resources["Assets\\Audio\\"] = InitializeList("Assets\\Audio");


		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			const std::string dirName = it->first;
			auto& assetList = it->second;
			for (auto& info : assetList)
			{
				CantDebugAPI::ResourceSetting(dirName.c_str(), info.Name.c_str(), &info.Pressed);
			}
		}

		m_prefabList = m_resources["Assets\\Prefabs\\"];
		for (auto &info : m_prefabList)
		{
			CantDebugAPI::PrefabButtonList(info.Name.c_str(), &info.Pressed);
		}


		// Material Generator Initialization
		CantDebugAPI::MaterialInfo debugInfo;
		debugInfo.Pressed = &m_materialInfo.GenerateFile;
		debugInfo.OutputPath = &m_materialInfo.OutputFileName;
		debugInfo.DiffuseTexturePath = &m_materialInfo.DiffuseTexturePath;
		debugInfo.NormalTexturePath = &m_materialInfo.NormalTexturePath;
		debugInfo.HeightTexturePath = &m_materialInfo.HeightTexturePath;
		debugInfo.DiffuseColor = &m_materialInfo.DiffuseColor.x;
		debugInfo.SpecularColor = &m_materialInfo.SpecularColor.x;
		for (auto it = m_resources["Assets\\Textures\\"].begin(); it != m_resources["Assets\\Textures\\"].end(); ++it)
		{
			m_materialInfo.TextureList.push_back(it->FullPath);
		}
		debugInfo.TextureList = &m_materialInfo.TextureList;
		CantDebugAPI::MaterialData(debugInfo);

	}

	void DebugManager::UpdateMaterialInfo()
	{
		if (m_materialInfo.GenerateFile)
		{
			// Output json here:
			using namespace rapidjson;
			Document doc;
			StringBuffer sb;
			PrettyWriter<StringBuffer> writer(sb);

			writer.StartObject();
			writer.Key("Diffuse_Texture_Dir"); writer.String(m_materialInfo.DiffuseTexturePath);
			writer.Key("Normal_Texture_Dir"); writer.String(m_materialInfo.NormalTexturePath);
			writer.Key("Height_Texture_Dir"); writer.String(m_materialInfo.HeightTexturePath);
			writer.Key("Diffuse_Color"); writer.StartArray();
			writer.Double(m_materialInfo.DiffuseColor.x); writer.Double(m_materialInfo.DiffuseColor.y); writer.Double(m_materialInfo.DiffuseColor.z); writer.Double(m_materialInfo.DiffuseColor.z);
			writer.EndArray();
			writer.Key("Specular_Color"); writer.StartArray();
			writer.Double(m_materialInfo.SpecularColor.x); writer.Double(m_materialInfo.SpecularColor.y); writer.Double(m_materialInfo.SpecularColor.z); writer.Double(m_materialInfo.SpecularColor.z);
			writer.EndArray();
			writer.EndObject();

			ofstream out_file;
			out_file.open("Assets\\Materials\\" + m_materialInfo.OutputFileName + ".json");
			if (out_file.is_open())
			{
				out_file << sb.GetString();
				out_file.close();
			}
			m_materialInfo.GenerateFile = false;
		}
	}

	void DebugManager::UpdateObjects()
	{
		bool editing = false;
		for (auto& objInfo : m_objectList)
		{
			if (objInfo.second.DoublePressed)
			{
				auto cam = m_pGameObjEditor->GetComponent<CameraComponent>();
				auto transform = m_pGameObjEditor->GetComponent<TransformComponent>();
				Vector3 position = objInfo.first->GetComponent<TransformComponent>()->GetPosition(); 
				Vector3 translation = cam->GetCamera().GetForward(); translation.Normalize(); translation = translation * -10.f;
				transform->SetLocalPosition(position);
				transform->Translate(translation);
			}
			objInfo.second.DoublePressed = false;


			if (!m_config.SelectionTool)
			{
				objInfo.second.Highlighted = false;
				objInfo.second.Pressed = false;
				continue;
			}

			if (m_meshObjects.find(objInfo.first) == m_meshObjects.end())
				continue;


			if (objInfo.second.Highlighted)
			{
				m_meshObjects[objInfo.first].m_aabb.DebugDraw(m_pAppRenderer, Vector4(0.5f, 0.f, 0.f, 0.5f));
			}
			if (objInfo.second.Pressed)
			{
				DisplayCompData(objInfo.first);
				m_meshObjects[objInfo.first].m_aabb.DebugDraw(m_pAppRenderer, Vector4(1.f, 0.f, 0.f, 0.f));
			}
			UpdateComponents(objInfo.first);
		}
	}

	void DebugManager::UpdateResources()
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			const std::string dirName = it->first.c_str();
			auto& assetList = it->second;
			for (auto& info : assetList)
			{
				if (info.Pressed != info._pressed)
				{
					if (info.Pressed)
					{
						if (dirName == "Assets\\Textures\\")
							m_pResourceManager->LoadTexture(info.FullPath);
						else if (dirName == "Assets\\Models\\")
							m_pResourceManager->LoadModel(info.FullPath);
						else if (dirName == "Scripts\\")
							m_pResourceManager->LoadScript(info.FullPath);
						else if (dirName == "Assets\\Prefabs\\")
							m_pResourceManager->LoadPrefab(info.FullPath);
						else if (dirName == "Assets\\Audio\\")
							m_pResourceManager->LoadAudio(info.FullPath);
					}
					else
					{
						if(m_pResourceManager->HasResource(info.FullPath))
							m_pResourceManager->FreeResource(info.FullPath);
					}
				}

				info.Pressed = m_pResourceManager->HasResource(StringId(info.FullPath));
				info._pressed = info.Pressed;
			}
		}
	}

	void DebugManager::UpdatePrefabCreation()
	{
		for (auto& goInfo : m_prefabList)
		{
			if (goInfo.Pressed)
			{
				Factory::LoadObject(m_pResourceManager->GetPrefab(goInfo.FullPath), goInfo.FullPath, "", m_pGameState->m_gameObjectMgr);
			}
		}
	}

	void DebugManager::UpdateState()
	{
		if(m_config.Pause_State)
			m_pGameState = m_pStateManager->m_stateStack[m_pStateManager->m_stateStack.size() - 2];
		else
			m_pGameState = m_pStateManager->m_stateStack[m_pStateManager->m_stateStack.size() - 1];
	}

	void DebugManager::Update()
	{
		UpdateObjects();
		UpdateResources();
		UpdatePrefabCreation();
		UpdateMaterialInfo();

		static bool _pauseState = false;
		static bool _selectionTool = false;
		static bool _CreateLevel = false;

		if (_pauseState != m_config.Pause_State)
		{
			if (m_config.Pause_State)
			{
				m_pStateManager->PushState("Assets\\Levels\\DebugPause.json");
				std::vector<GameObject*> m;
				m_pStateManager->m_stateStack[m_pStateManager->m_stateStack.size() - 1]->m_gameObjectMgr->CompleteGORegistration(m_pGameObjEditor, m_pAppRenderer, m_pResourceManager, m);
			}
			else
			{
				m_pStateManager->m_stateStack[m_pStateManager->m_stateStack.size() - 1]->m_gameObjectMgr->m_gameObjects.clear();
				m_pStateManager->PopState();
			}
		}

		if (_CreateLevel != m_config.Create_Level)
		{
			if (m_config.Create_Level)
				LevelToJson("Assets\\Levels\\Testing.json");
			m_config.Create_Level = false;
		}

		CantDebugAPI::EditorSetting("Pause", &m_config.Pause_State);
		CantDebugAPI::EditorSetting("SelectionTool", &m_config.SelectionTool);
		CantDebugAPI::EditorSetting("Create Level", &m_config.Create_Level);


		Matrix model;
		MeshComponent* mesh;

		for (auto& go : m_meshObjects)
		{
			// updating aabb tree
			mesh = go.first->GetComponent<MeshComponent>();
			Aabb& aabb = go.second.m_aabb;
			aabb = mesh->GetModel()->GetAABB();
			model = go.first->GetComponent<TransformComponent>()->GetModel();
			aabb.Transform(model);
			SpatialPartitionData data(go.first, aabb);
			m_AabbTree.UpdateData(go.second.m_key, data);
		}		

		// Update State
		_pauseState = m_config.Pause_State;
		_selectionTool = m_config.SelectionTool;
		_CreateLevel = m_config.Create_Level;
		UpdateState();

	}

	std::vector<GameObject*> DebugManager::GetSelectedObjects()
	{
		std::vector<GameObject*> result;
		for (auto it = m_objectList.begin(); it != m_objectList.end(); ++it)
		{
			if (it->second.Pressed)
				result.push_back(it->first);
		}
		return result;
	}

	std::vector<GameObject*> DebugManager::RayCast()
	{
		if (m_pGameObjEditor == nullptr)
			return std::vector<GameObject*>();
		CameraComponent* camera_comp = m_pGameObjEditor->GetComponent<CameraComponent>();
		Camera& cam = camera_comp->GetCamera();

		float x = ((m_pointerPosition.x * 2.f) / m_scrDimensions.x) - 1.f;
		float y = 1.f - ((m_pointerPosition.y * 2.f) / m_scrDimensions.y);
		float z = 1.f;

		Vector3 ray_nds(x, y, z);
		Vector4 ray_clip(ray_nds.x, ray_nds.y, -1.f, 1.f);

		//to 4d eye coordinates
		Vector4 ray_eye_dir_pos = DirectX::XMVector4Transform(ray_clip, cam.GetInvProjectionMatrix());

		Vector4 ray_eye_dir = Vector4(ray_eye_dir_pos.x, ray_eye_dir_pos.y, -1.f, 0.f);

		//to world coord
		Vector3 ray_world_dir = DirectX::XMVector4Transform(ray_eye_dir, cam.GetInvViewMatrix());
		ray_world_dir.Normalize();

		DEBUG_LOG("Position: "); DEBUG_LOGVec3(cam.GetCameraPosition());
		DEBUG_LOG("Direction: "); DEBUG_LOGVec3(ray_world_dir);

		CastResults result;
		RayCant ray(cam.GetCameraPosition(), ray_world_dir);
		m_AabbTree.CastRay(ray, result);

		std::vector<GameObject*> results;
		for (auto& result : result.m_Results)
		{
			GameObject* go = static_cast<GameObject*>(result.m_ClientData);
			results.push_back(go);
			//DEBUG_LOG("Key: %s\n", go->GetTag().c_str());
		}
		return results;
	}
	
	void DebugManager::RegisterObject(const GameObjectCreated* e)
	{
		GameObject* go = e->m_pGameObject;
		if (go->GetTag() == "lvleditor")
		{
			m_pGameObjEditor = go;
			return;
		}

		// Register into Objects
		Info objInfo; 
		objInfo.ID = go->GetId();
		if (go->GetTag() == "")	objInfo.Name = std::to_string(go->GetId());
		else objInfo.Name = go->GetTag();
		objInfo.Pressed = false; objInfo.DoublePressed = false; objInfo.Highlighted = false;
		m_objectList.insert(std::make_pair(go,objInfo));
		CantDebugAPI::ObjectButtonList(std::to_string(objInfo.ID).c_str(), objInfo.Name.c_str(), &m_objectList[go].Pressed, &m_objectList[go].DoublePressed, true);
		RegisterComponents(go);

		// Register dynamic AABB for raycast
		static unsigned int key = 0;
		MeshComponent* mesh = go->GetComponent<MeshComponent>();
		if (mesh == nullptr || go->HasComponent<AnimationComponent>())
			return;
		SpatialPartitionData data1;
		Aabb aabb = mesh->GetModel()->GetAABB();
		m_AabbTree.InsertData(key, SpatialPartitionData(go, aabb));
		GameObjectData data(key, aabb);
		m_meshObjects.insert(std::make_pair(e->m_pGameObject, data));
	}

	void DebugManager::RegisterComponents(GameObject* go)
	{
		CompInfo info;
		CantDebugAPI::PropertyInfo debugInfo;
		auto& components = m_objectList[go].Components;
		auto transform = go->GetComponent<TransformComponent>();
		if (transform)
		{
			info.compName = "Transform"; info.propName = "Position"; info.data.vec3 = transform->GetPosition();  info.type = CantDebugAPI::VEC3; components.push_back(info);
			info.compName = "Transform"; info.propName = "Rotation"; info.data.vec3 = transform->GetRotation();  info.type = CantDebugAPI::VEC3; components.push_back(info);
			info.compName = "Transform"; info.propName = "Scale";	 info.data.vec3 = transform->GetScale();	 info.type = CantDebugAPI::VEC3; components.push_back(info);
		}
		auto rigidbdy = go->GetComponent<RigidbodyComponent>();
		if (rigidbdy)
		{
			info.compName = "RigidBody"; info.propName = "Mass"; info.data.f = rigidbdy->GetMass(); info.type = CantDebugAPI::FLOAT; components.push_back(info);
		}
		auto renderer = go->GetComponent<RendererComponent>();
		if (renderer)
		{
			info.compName = "Renderer"; info.propName = "MaterialId"; info.strVal = renderer->m_materialId.getName(); info.type = CantDebugAPI::STRING; components.push_back(info);
		}

	}

	void DebugManager::UnregisterObject(const GameObjectDestroyed* e)
	{
		GameObject* go = e->m_pGameObject;
		if (m_objectList.find(go) != m_objectList.end())
		{
			CantDebugAPI::ObjectButtonList(std::to_string(m_objectList[go].ID).c_str(), m_objectList[go].Name.c_str(), &m_objectList[go].Pressed, &m_objectList[go].DoublePressed, false);
			m_objectList.erase(go);
		}

		m_meshObjects.erase(go);
	}

	void DebugManager::OnClick(const MouseClickEvent* e)
	{
		if (m_config.SelectionTool)
		{
			if (e->m_button == SDL_BUTTON_LEFT && e->m_state)
			{
				if (!m_config.Is_Ctrl)
				{
					for (auto& go : m_objectList)
						go.second.Pressed = false;
				}

				std::vector<GameObject*> raycast = RayCast();
				if (raycast.size() > 0)
				{
					auto it = m_objectList.find(*raycast.begin());
					if (it == m_objectList.end())
						return;
					it->second.Pressed = !it->second.Pressed;
				}
			}
		}
	}

	void DebugManager::OnMotion(const MouseMotionEvent* e)
	{
		m_pointerPosition = e->m_position;
		if (m_config.SelectionTool)
		{
			for (auto& go : m_objectList)
				go.second.Highlighted = false;

			std::vector<GameObject*> raycast = RayCast();
			if (raycast.size() > 0)
			{
				auto it = m_objectList.find(*raycast.begin());
				if (it == m_objectList.end())
					return;
				it->second.Highlighted = true;
			}
		}
		
	}

	void DebugManager::OnScreenResize(const WindowSizeEvent* e)
	{
		m_scrDimensions.x = static_cast<float>(e->m_width);
		m_scrDimensions.y = static_cast<float>(e->m_height);
	}

	void DebugManager::OnKey(const KeyEvent* e)
	{
		switch (e->m_scancode)
		{
		case SDL_SCANCODE_LCTRL:
			m_config.Is_Ctrl = e->m_press;
			break;
		case SDL_SCANCODE_P:
			if(e->m_press)
			m_config.Pause_State = !m_config.Pause_State;
			break;
		case SDL_SCANCODE_DELETE:
		{
			if (!e->m_press)
				break;

			auto it = m_objectList.begin();
			GameObject* go;
			while (it != m_objectList.end())
			{
				if (it->second.Pressed)
				{
					go = it->first;
					CantDebugAPI::ObjectButtonList(std::to_string(m_objectList[go].ID).c_str(), m_objectList[go].Name.c_str(), &m_objectList[go].Pressed, &m_objectList[go].DoublePressed, false);
					it = m_objectList.erase(it);
					if (m_meshObjects.find(go) != m_meshObjects.end())
					{
						m_AabbTree.RemoveData(m_meshObjects[go].m_key);
						m_meshObjects.erase(go);
					}
					go->Destroy();
				}
				else
					++it;
			}
			break;
		}
		}
	}

	void DebugManager::DisplayCompData(GameObject* go)
	{
		auto& compList = m_objectList[go].Components;
		auto it = compList.begin();
		CantDebugAPI::PropertyInfo debugInfo;
		debugInfo.goName = m_objectList[go].Name;

		TransformComponent* transform = go->GetComponent<TransformComponent>();
		if (transform)
		{
			debugInfo.compName = it->compName;
			// Transform 
			transform->SetLocalPosition(it->data.vec3);
			debugInfo.propName = it->propName;
			debugInfo.f = &it->data.vec3.x; debugInfo.t = CantDebugAPI::VEC3; debugInfo.min = -100.f; debugInfo.max = 100.f;
			CantDebugAPI::ComponentData(debugInfo); ++it;

			transform->SetLocalRotation(it->data.vec3.x, it->data.vec3.y, it->data.vec3.z);
			debugInfo.propName = it->propName;
			debugInfo.f = &it->data.vec3.x; debugInfo.t = CantDebugAPI::VEC3; debugInfo.min = -180.f; debugInfo.max = 180.f;
			CantDebugAPI::ComponentData(debugInfo); ++it;

			transform->Scale(it->data.vec3);
			debugInfo.propName = it->propName;
			debugInfo.f = &it->data.vec3.x; debugInfo.t = CantDebugAPI::VEC3; debugInfo.min = 0.f; debugInfo.max = 50.f;
			CantDebugAPI::ComponentData(debugInfo); ++it;
		}
		RigidbodyComponent* rigidBody = go->GetComponent<RigidbodyComponent>();
		if (rigidBody)
		{
			debugInfo.compName = it->compName;

			rigidBody->SetMass(it->data.f);
			debugInfo.propName = it->propName;
			debugInfo.f = &it->data.f; debugInfo.t = CantDebugAPI::FLOAT; debugInfo.min = 0.f; debugInfo.max = 50.f;
			CantDebugAPI::ComponentData(debugInfo); ++it;
		}
		auto renderer = go->GetComponent<RendererComponent>();
		if (renderer)
		{
			debugInfo.compName = it->compName;
			renderer->m_materialId = it->strVal;
			debugInfo.propName = it->propName;
			debugInfo.propValString = &it->strVal; debugInfo.t = CantDebugAPI::STRING;
			CantDebugAPI::ComponentData(debugInfo); ++it;
		}

	}
	
	void DebugManager::UpdateComponents(GameObject* go)
	{
		auto& compList = m_objectList[go].Components;
		auto it = compList.begin();
		TransformComponent* transform = go->GetComponent<TransformComponent>();
		if (transform)
		{
			it->data.vec3 = transform->GetPosition(); ++it;
			it->data.vec3 = transform->GetRotation(); ++it;
			it->data.vec3 = transform->GetScale(); ++it;
		}
		RigidbodyComponent* rigidBody = go->GetComponent<RigidbodyComponent>();
		if (rigidBody)
		{
			it->data.f = rigidBody->GetMass(); ++it;
		}
		auto renderer = go->GetComponent<RendererComponent>();
		if (renderer)
		{
			it->strVal = renderer->m_materialId.getName(); ++it;
		}
	}

	void DebugManager::LevelToJson(const std::string& levelPath)
	{
		using namespace rapidjson;
		Document doc; 
		StringBuffer sb;
		PrettyWriter<StringBuffer> writer(sb);
		writer.StartObject();
#pragma region RESOURCES
		writer.Key("Resources");
		writer.StartObject();
		writer.Key("Textures");
		writer.StartArray();
		for (auto& info : m_resources["Assets\\Textures\\"])
			if(info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();

		writer.Key("Models");
		writer.StartArray();
		for (auto& info : m_resources["Assets\\Models\\"])
			if (info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();

		writer.Key("Materials");
		writer.StartArray();
		for (auto& info : m_resources["Assets\\Materials\\"])
			if (info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();

		writer.Key("Scripts");
		writer.StartArray();
		for (auto& info : m_resources["Scripts\\"])
			if (info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();

		writer.Key("Prefabs");
		writer.StartArray();
		for (auto& info : m_resources["Assets\\Prefabs\\"])
			if (info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();

		writer.Key("Audio");
		writer.StartArray();
		for (auto& info : m_resources["Assets\\Audio\\"])
			if (info.Pressed) writer.String(info.FullPath.c_str());
		writer.EndArray();
		writer.EndObject();
#pragma endregion
#pragma region OBJECTS
		writer.Key("Objects");
		writer.StartArray();
		for (auto it = m_objectList.begin(); it != m_objectList.end(); ++it)
		{
			if (it->first->GetPrefabName().empty())
				continue;
			writer.StartObject();
			writer.Key("tag"); writer.String(it->first->GetTag().c_str());
			writer.Key("prefab"); writer.String(it->first->GetPrefabName().c_str());
			writer.Key("overrides"); ReadOverrides(it->first, writer);
			writer.EndObject();
		}
		writer.EndArray();
#pragma endregion
		writer.EndObject();
		const std::string jsonString = sb.GetString();
		ofstream out_file;
		out_file.open(levelPath);
		if (out_file.is_open())
		{
			out_file << sb.GetString();
			out_file.close();
		}

	}

	template<typename T>
	void WriteComponentOverride(T* component, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		rttr::instance var(component);
		writer.Key(var.get_derived_type().get_name().to_string());
		CantReflect::WriteRecursive(component, writer);
	}

	void DebugManager::ReadOverrides(GameObject* go, rapidjson::PrettyWriter<rapidjson::StringBuffer>& levelWriter)
	{
		using namespace rapidjson;

		// Read the object in its current state completely.
		// Get the prefabdoc from the resmgr
		const std::string & prefabName = go->GetPrefabName();
		StringId prefabId = StringId(prefabName);
		const std::string prefabJson = m_pResourceManager->GetPrefab(prefabId);
		rapidjson::Document prefabDoc;
		prefabDoc.Parse(prefabJson);
		assert(!prefabDoc.HasParseError());
		
		StringBuffer sb;
		PrettyWriter<StringBuffer> writer(sb);
		writer.StartObject();
		if (go->HasComponent<TransformComponent>())
			WriteComponentOverride(go->GetComponent<TransformComponent>(), writer);
		if (go->HasComponent<RigidbodyComponent>())
			WriteComponentOverride(go->GetComponent<RigidbodyComponent>(), writer);
		if (go->HasComponent<RendererComponent>())
			WriteComponentOverride(go->GetComponent<RendererComponent>(), writer);
		if (go->HasComponent<MeshComponent>())
			WriteComponentOverride(go->GetComponent<MeshComponent>(), writer);
		if (go->HasComponent<LightComponent>())
			WriteComponentOverride(go->GetComponent<LightComponent>(), writer);
		if (go->HasComponent<CameraComponent>())
			WriteComponentOverride(go->GetComponent<CameraComponent>(), writer);
		if (go->HasComponent<ParticleEmitterComponent>())
			WriteComponentOverride(go->GetComponent<ParticleEmitterComponent>(), writer);
		if (go->HasComponent<HaloEffectComponent>())
			WriteComponentOverride(go->GetComponent<HaloEffectComponent>(), writer);
		if (go->HasComponent<AnimationComponent>())
			WriteComponentOverride(go->GetComponent<AnimationComponent>(), writer);
		writer.EndObject();

		Document overrideDoc;
		overrideDoc.Parse(sb.GetString());
		assert(!overrideDoc.GetParseError());

		auto prefabList = prefabDoc.GetObjectA();
		auto overrideList = overrideDoc.GetObjectA();

		Factory::RecursiveRead(prefabList, overrideList, prefabDoc);
		prefabDoc.Accept(levelWriter);
	}
}

#endif