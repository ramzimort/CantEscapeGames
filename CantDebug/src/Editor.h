#pragma once

#include <unordered_map>

struct Info
{
	bool Include = false;
	std::string Name;
};

class Editor
{
	typedef std::pair<std::string, bool> ResourceInclude;
	typedef std::unordered_map<std::string, std::vector<Info>> ResourceMap;
	typedef std::unordered_map<std::string, std::vector<bool>> IncludeMap;
public:
	Editor(const std::string& texturesDir,
		const std::string& modelDir,
		const std::string& materialDir,
		const std::string& scriptDir,
		const std::string& prefabDir,
		const std::string& audioDir);

	~Editor();

	void LoadResources();
	void Update();

private:
	ResourceMap m_resourceMap;
	IncludeMap m_includeMap;

	const std::string TextureFolder;
	const std::string ModelFolder;
	const std::string MaterialFolder;
	const std::string ScriptFolder;
	const std::string PrefabFolder;
	const std::string AudioFolder;
};