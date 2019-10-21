#pragma once

#include "CheckboxQueue.h"
#include <unordered_map>

struct Info
{
	std::string Name;
	bool* Include;
};

struct ButtonInfo
{

};

class Editor
{
	typedef std::pair<std::string, bool> ResourceInclude;
	typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;
	typedef std::vector<Info> PrefabButtons;
public:
	Editor();
	~Editor();

	void UpdateResources(const char* dir, const char* asset, bool* pFlag);
	void UpdatePrefabs(const char* prefabName, bool* p_buttonState);
	void UpdateSettings(const char* checkboxName, bool* pFlag);
	void Update();
	void Clear();
private:
	ResourceMap m_resourceMap;
	CheckboxQueue m_queue;
	PrefabButtons m_prefabButtons;
	PrefabButtons m_objects;

};