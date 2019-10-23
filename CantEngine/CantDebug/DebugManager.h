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
	struct MaterialInfo
	{
		bool GenerateFile;
		std::string OutputFileName;

		MaterialInfo() = default;
		std::string DiffuseTexturePath = "";
		std::string NormalTexturePath = "";
		std::string HeightTexturePath = "";
		Vector4 DiffuseColor;
		Vector4 SpecularColor;

		std::vector<std::string> TextureList;
	};


	struct DebugConfig
	{
		bool SelectionTool = false;
		bool Is_Ctrl = false;
		bool Pause_State = false;
		bool Create_Level = false;
	};

	union CompDataUnion
	{
		CompDataUnion() { memset(this, 0, sizeof(CompDataUnion)); }
		size_t _int;
		float f;
		Vector2 vec2;
		Vector3 vec3;
		Vector4 vec4;
	};

	struct CompInfo
	{
		CompDataUnion data;
		std::string strVal;
		CantDebugAPI::type type;
		std::string compName;
		std::string propName;
	};

	struct Info
	{
		Info() : FullPath(""), Name(""), Pressed(false), DoublePressed(false), Highlighted(false), _pressed(false), ID(-1), Components(std::list<CompInfo>()) { }

		std::string FullPath;
		std::string Name;
		bool Pressed = false;
		bool DoublePressed = false;
		bool Highlighted = false;
		bool _pressed;
		size_t ID;
		std::list<CompInfo> Components;
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
		//typedef std::unordered_map<std::string, 

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
		void UpdateMaterialInfo();

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
		
		void RegisterComponents(GameObject* go);
		void DisplayCompData(GameObject* go);
		void UpdateComponents(GameObject* go);
		void LevelToJson(const std::string& levelName);
		void ReadOverrides(GameObject* go, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	private:
		Vector2 m_scrDimensions = { 1280,720 };
		Vector2 m_pointerPosition;
		DebugConfig m_config;
		GameObject* m_pGameObjEditor;

		ObjectList m_objectList;
		ResourceMap m_resources;
		PrefabButtonList m_prefabList;
		MaterialInfo m_materialInfo;

		MeshObjectList m_meshObjects;
		DynamicAabbTree m_AabbTree;

		State* m_pGameState;
		AppRenderer* m_pAppRenderer;
		ResourceManager* m_pResourceManager;
		StateManager* m_pStateManager;
	};
}


#endif // DEVELOPER

