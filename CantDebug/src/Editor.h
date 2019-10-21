#pragma once

#include "CheckboxQueue.h"
#include <unordered_map>
#include <DirectXTK/SimpleMath.h>

struct Info
{
	std::string ID;
	std::string Name;
	bool* Pressed;
	bool* DoubleClicked;
};

struct ComponentInfo
{
	std::string CompName;
	std::string PropName;
	float* vec3;
	float min;
	float max;
};

class Editor
{
	typedef std::pair<std::string, bool> ResourceInclude;
	typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;
	typedef std::vector<Info> PrefabButtons;
	typedef std::unordered_map<std::string, std::queue<ComponentInfo>> ComponentData;
public:
	Editor();
	~Editor();
	void UpdateComponents(const char* compName, const char* propName, float* vec, float min, float max);
	void UpdateObjects(const char* id, const char* name, bool* pClicked, bool* pDoubleClicked, bool created);
	void UpdateResources(const char* dir, const char* asset, bool* pFlag);
	void UpdatePrefabs(const char* prefabName, bool* p_buttonState);
	void UpdateSettings(const char* checkboxName, bool* pFlag);
	void Update();
	void UpdateComponentWindow();
	void Clear();
private:
	ResourceMap m_resourceMap;
	CheckboxQueue m_queue;
	PrefabButtons m_prefabButtons;
	PrefabButtons m_objects;
	ComponentData m_components;
};