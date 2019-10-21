
#ifdef DEVELOPER
#include "DebugManager.h"
#include "Managers/GameObjectManager.h"
#include "Components/AllComponentHeaders.h"
#include "GameObjects/GameObject.h"
#include "Managers/EventManager.h"
#include "Graphics/Camera.h"
#include "Directory/Directory.h"
#include "Factory/Factory.h"

namespace CantDebug
{
	using namespace std;
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

		m_pAppRenderer = pAppRenderer;
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
	}

	void DebugManager::UpdateObjects()
	{
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
				m_meshObjects[objInfo.first].m_aabb.DebugDraw(m_pAppRenderer, Vector4(1.f, 0.f, 0.f, 0.f));
			}
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
				Factory::LoadObject(m_pResourceManager->GetPrefab(goInfo.FullPath), "", m_pGameState->m_gameObjectMgr);
				goInfo.Pressed = false;
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

		static bool _pauseState = false;
		static bool _selectionTool = false;

		if (_pauseState != m_config.Pause_State)
		{
			if (m_config.Pause_State)
				m_pStateManager->PushState("Assets\\Levels\\DebugPause.json");
			else
				m_pStateManager->PopState();
		}

		CantDebugAPI::EditorSetting("Pause", &m_config.Pause_State);
		CantDebugAPI::EditorSetting("SelectionTool", &m_config.SelectionTool);

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

		// Register dynamic AABB for raycast
		static unsigned int key = 0;
		MeshComponent* mesh = go->GetComponent<MeshComponent>();
		if (mesh == nullptr)
			return;
		SpatialPartitionData data1;
		Aabb aabb = mesh->GetModel()->GetAABB();
		m_AabbTree.InsertData(key, SpatialPartitionData(go, aabb));
		GameObjectData data(key, aabb);
		m_meshObjects.insert(std::make_pair(e->m_pGameObject, data));
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

}

#endif