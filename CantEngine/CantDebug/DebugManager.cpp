
#ifdef DEVELOPER
#include "DebugManager.h"

#include "Components/AllComponentHeaders.h"
#include "GameObjects/GameObject.h"
#include "Managers/EventManager.h"
#include "Graphics/Camera.h"




namespace CantDebug
{
	DebugManager::DebugManager(AppRenderer* pAppRenderer) : 
		m_pGameObjEditor(nullptr)
	{
		EventManager::Get()->SubscribeEvent<GameObjectCreated>(this, std::bind(&DebugManager::RegisterObject, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<GameObjectDestroyed>(this, std::bind(&DebugManager::UnregisterObject, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<MouseClickEvent>(this, std::bind(&DebugManager::OnClick, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<MouseMotionEvent>(this, std::bind(&DebugManager::OnMotion, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<WindowSizeEvent>(this, std::bind(&DebugManager::OnScreenResize, this, std::placeholders::_1));
		EventManager::Get()->SubscribeEvent<KeyEvent>(this, std::bind(&DebugManager::OnKey, this, std::placeholders::_1));

		m_pAppRenderer = pAppRenderer;
	}

	DebugManager::~DebugManager() { }

	void DebugManager::Update()
	{
		Matrix model;
		MeshComponent* mesh;
		
		for (auto& go : m_objects)
		{
			// updating aabb tree
			mesh = go.first->GetComponent<MeshComponent>();
			Aabb& aabb = go.second.m_aabb;
			aabb = mesh->GetModel()->GetAABB();
			model = go.first->GetComponent<TransformComponent>()->GetModel();
			aabb.Transform(model);
			SpatialPartitionData data(go.first, aabb);
			m_AabbTree.UpdateData(go.second.m_key, data);
			
			if (go.second.m_highlighted)
			{
				aabb.DebugDraw(m_pAppRenderer, Vector4(0.5f, 0.f, 0.f, 0.5f));
			}
			if (go.second.m_selected)
			{
				aabb.DebugDraw(m_pAppRenderer, Vector4(1.f, 0.f, 0.f, 0.f));
			}
		}
	}

	std::vector<GameObject*> DebugManager::GetSelectedObjects()
	{
		std::vector<GameObject*> result;
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (it->second.m_selected)
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

		static unsigned int key = 0;
		MeshComponent* mesh = e->m_pGameObject->GetComponent<MeshComponent>();

		if (mesh == nullptr)
			return;

		// register with debug dynamic aabb
		SpatialPartitionData data1;
		Aabb aabb = mesh->GetModel()->GetAABB();
		m_AabbTree.InsertData(key, SpatialPartitionData(go, aabb));

		// Register in list
		GameObjectData data(key, aabb);
		data.m_selected = false;
		data.m_highlighted = false;
		m_objects.insert(std::make_pair(e->m_pGameObject, data));
	}

	void DebugManager::UnregisterObject(const GameObjectDestroyed* event)
	{
		m_objects.erase(event->m_pGameObject);
	}

	void DebugManager::OnClick(const MouseClickEvent* e)
	{
		if (e->m_button == SDL_BUTTON_LEFT && e->m_state)
		{
			if (!m_config.Is_Ctrl)
			{
				for (auto& go : m_objects)
					go.second.m_selected = false;
			}

			std::vector<GameObject*> raycast = RayCast();
			if (raycast.size() > 0)
			{
				auto it = m_objects.find(*raycast.begin());
				if (it == m_objects.end())
					return;
				it->second.m_selected = true;
			}
		}
	}

	void DebugManager::OnMotion(const MouseMotionEvent* e)
	{
		m_pointerPosition = e->m_position;
		for (auto& go : m_objects)
			go.second.m_highlighted = false;

		std::vector<GameObject*> raycast = RayCast();
		if (raycast.size() > 0)
		{
			auto it = m_objects.find(*raycast.begin());
			if (it == m_objects.end())
				return;
			it->second.m_highlighted = true;
		}
	}

	void DebugManager::OnScreenResize(const WindowSizeEvent* e)
	{
		m_scrDimensions.x = static_cast<float>(e->m_width);
		m_scrDimensions.y = static_cast<float>(e->m_height);
	}

	void DebugManager::OnKey(const KeyEvent* e)
	{
		// On Release
		if (e->m_press)
		{
			switch (e->m_scancode)
			{
			case SDL_SCANCODE_LCTRL:
				m_config.Is_Ctrl = e->m_press;
				break;

			case SDL_SCANCODE_DELETE:
			{
				auto it = m_objects.begin();
				GameObject* go;
				while (it != m_objects.end())
				{
					if (it->second.m_selected)
					{
						go = it->first;
						it = m_objects.erase(it);
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

}

#endif