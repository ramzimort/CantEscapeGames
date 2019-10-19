#pragma once

#ifdef DEVELOPER
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Physics/Geometry/Aabb.h"
#include "GameObjects/GameObject.h"

#include "Events/GameObject/GameObjectEvents.h"
#include "Events/Input/Input.h"
namespace CantDebug
{
	struct DebugConfig
	{
		bool SelectionTool;
		bool Is_Ctrl;
	};

	struct GameObjectData
	{
		GameObjectData(const GameObjectData& rhs) :
			m_aabb(rhs.m_aabb), m_key(rhs.m_key) { }
		GameObjectData(unsigned int key, const Aabb& aabb) :
			m_aabb(aabb), m_key(key) { }

		Aabb m_aabb;
		unsigned int m_key;
		bool m_selected;
		bool m_highlighted;
	};

	class DebugManager
	{
		typedef std::unordered_map<GameObject*, GameObjectData> ObjectList;
	public:
		DebugManager(AppRenderer* pAppRenderer);
		~DebugManager();

		void Update();

		std::vector<GameObject*> GetSelectedObjects();
		std::vector<GameObject*> RayCast();

		void RegisterObject(const GameObjectCreated* e);
		void UnregisterObject(const GameObjectDestroyed* e);

		void OnClick(const MouseClickEvent* e);
		void OnMotion(const MouseMotionEvent* e);
		void OnScreenResize(const WindowSizeEvent* e);
		void OnKey(const KeyEvent* e);

	private:
		Vector2 m_scrDimensions = { 1280,720 };
		Vector2 m_pointerPosition;
		GameObject* m_pGameObjEditor;

		ObjectList m_objects;

		DebugConfig m_config;
		DynamicAabbTree m_AabbTree;

		AppRenderer* m_pAppRenderer;
	};
}


#endif // DEVELOPER

