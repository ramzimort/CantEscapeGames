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
	};

	struct Info
	{
		std::string FullPath;
		std::string Name;
		bool Pressed = false;
		bool DoublePressed = false;
		bool Highlighted = false;
		bool _pressed;
		size_t ID;
	};

	struct GameObjectData
	{
		GameObjectData() = default;
		GameObjectData(const GameObjectData& rhs) :
			m_aabb(rhs.m_aabb), m_key(rhs.m_key) { }
		GameObjectData(unsigned int key, const Aabb& aabb) :
			m_aabb(aabb), m_key(key) { }

		Aabb m_aabb;
		unsigned int m_key;
	};

	class DebugManager
	{
		typedef std::unordered_map<GameObject*, Info> ObjectList;
		typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;
		typedef std::vector<Info> PrefabButtonList;
		typedef std::unordered_map<GameObject*, GameObjectData> MeshObjectList;

	public:
		DebugManager(AppRenderer* pAppRenderer, ResourceManager* pResourceManager, StateManager* pStateManager);
		~DebugManager();

		void Initialize();

		// Update
		void Update();
		void UpdateObjects();
		void UpdateResources();
		void UpdatePrefabCreation();
		void UpdateState();

		// Helpers
		std::vector<GameObject*> GetSelectedObjects();
		std::vector<GameObject*> RayCast();

		// Gameobjects
		void RegisterObject(const GameObjectCreated* e);
		void UnregisterObject(const GameObjectDestroyed* e);

		// Events
		void OnClick(const MouseClickEvent* e);
		void OnMotion(const MouseMotionEvent* e);
		void OnScreenResize(const WindowSizeEvent* e);
		void OnKey(const KeyEvent* e);

	private:
		Vector2 m_scrDimensions = { 1280,720 };
		Vector2 m_pointerPosition;
		DebugConfig m_config;
		GameObject* m_pGameObjEditor;

		ObjectList m_objectList;
		ResourceMap m_resources;
		PrefabButtonList m_prefabList;

		MeshObjectList m_meshObjects;
		DynamicAabbTree m_AabbTree;

		State* m_pGameState;
		AppRenderer* m_pAppRenderer;
		ResourceManager* m_pResourceManager;
		StateManager* m_pStateManager;
	};
}


#endif // DEVELOPER

