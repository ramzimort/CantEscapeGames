#pragma once

#include "CheckboxQueue.h"
#include <unordered_map>
#include <DirectXTK/SimpleMath.h>
#include "..\CantDebug\CantDebug.h"
#include <map>

struct Info
{
	std::string ID;
	std::string Name;
	bool* Pressed;
	bool* DoubleClicked;
};

using namespace CantDebugAPI;
class Editor
{
	typedef std::pair<std::string, bool> ResourceInclude;
	typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;
	typedef std::vector<Info> PrefabButtons;
	typedef std::map<std::string, std::map<std::string, std::list<PropertyInfo>>> ComponentData;
public:
	Editor();
	~Editor();
	void RegisterSettings(DebugConfig config);
	void UpdateComponents(PropertyInfo info);
	void UpdateObjects(const char* id, const char* name, bool* pClicked, bool* pDoubleClicked, bool created);
	void UpdateResources(const char* dir, const char* asset, bool* pFlag);
	void UpdatePrefabs(const char* prefabName, bool* p_buttonState);
	void Update();
	void UpdateComponentWindow();
	void Clear();
private:
	ResourceMap m_resourceMap;
	PrefabButtons m_prefabButtons;
	PrefabButtons m_objects;
	ComponentData m_components;
	DebugConfig m_settings;
};