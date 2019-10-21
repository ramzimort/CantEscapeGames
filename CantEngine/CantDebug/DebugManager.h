#pragma once

#ifdef DEVELOPER
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Physics/Geometry/Aabb.h"
#include "GameObjects/GameObject.h"
#include "Events/GameObject/GameObjectEvents.h"
#include "Events/Input/Input.h"
#include "Events/State/StateEvents.h"
#include "Managers/StateManager.h"
#include "Managers/ResourceManager.h"

namespace CantDebug
{
	struct DebugConfig
	{
		bool SelectionTool = false;
		bool Is_Ctrl = false;
		bool Pause_State = false;

		//uint32_t MSAA_SAMPLE_COUNT;
		//float MSAA_Filter_Size;
		//bool Draw_Debug_Mesh_AABB_Flag;
		//bool Draw_Debug_Mesh_Sphere_Flag;
		//int Moment_Shadow_Map_Blur_Width;

	};

	struct Info
	{
		std::string Name;
		bool Include = false;
		bool _include;
	};

	struct GameObjectData
	{
		GameObjectData(const GameObjectData& rhs) :
			m_aabb(rhs.m_aabb), m_key(rhs.m_key) { }
		GameObjectData(unsigned int key, const Aabb& aabb) :
			m_aabb(aabb), m_key(key) { }

		Aabb m_aabb;
		unsigned int m_key;
		bool m_selected = false;
		bool m_highlighted = false;
	};

	class DebugManager
	{
		typedef std::unordered_map<GameObject*, GameObjectData> ObjectList;
		typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;

	public:
		DebugManager(AppRenderer* pAppRenderer, ResourceManager* pResourceManager, StateManager* pStateManager);
		~DebugManager();

		void LoadResources();
		void UpdateResources();

		void Update();

		std::vector<GameObject*> GetSelectedObjects();
		std::vector<GameObject*> RayCast();

		void RegisterObject(const GameObjectCreated* e);
		void UnregisterObject(const GameObjectDestroyed* e);
		void CreateObject(const std::string& prefabName);

		void OnClick(const MouseClickEvent* e);
		void OnMotion(const MouseMotionEvent* e);
		void OnScreenResize(const WindowSizeEvent* e);
		void OnKey(const KeyEvent* e);

	private:
		Vector2 m_scrDimensions = { 1280,720 };
		Vector2 m_pointerPosition;
		GameObject* m_pGameObjEditor;
		GameObjectManager* m_pLevelGOManager;

		ObjectList m_objects;
		ResourceMap m_resources;

		DebugConfig m_config;
		DynamicAabbTree m_AabbTree;


		AppRenderer* m_pAppRenderer;
		ResourceManager* m_pResourceManager;
		StateManager* m_pStateManager;
	};
}


#endif // DEVELOPER

