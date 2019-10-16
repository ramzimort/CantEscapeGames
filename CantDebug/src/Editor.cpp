
#include "Editor.h"
#include "Directory.h"
#include "..\imgui\imgui.h"

using namespace std;
vector<Info> InitializeList(const vector<string>& list)
{
	vector<Info> result;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		Info info; 
		info.Name = *it;
		info.Include = false;
		result.push_back(info);
	}
	return result;
}

Editor::Editor( const std::string& texturesDir,	const std::string& modelDir, const std::string& materialDir,	const std::string& scriptDir, const std::string& prefabDir, const std::string& audioDir) :
	TextureFolder(texturesDir), 
	ModelFolder(modelDir), 
	MaterialFolder(materialDir), 
	ScriptFolder(scriptDir), 
	PrefabFolder(prefabDir), 
	AudioFolder(audioDir) 
{ 
	LoadResources();
}

Editor::~Editor()
{ }

void Editor::LoadResources()
{
	m_resourceMap[TextureFolder] = InitializeList(GetAllObjects(TextureFolder));
	m_resourceMap[ModelFolder] = InitializeList(GetAllObjects(ModelFolder));
	m_resourceMap[MaterialFolder] =  InitializeList(GetAllObjects(MaterialFolder));
	m_resourceMap[ScriptFolder] = InitializeList(GetAllObjects(ScriptFolder));
	m_resourceMap[PrefabFolder] = InitializeList(GetAllObjects(PrefabFolder));
	m_resourceMap[AudioFolder] = InitializeList(GetAllObjects(AudioFolder));
}


void Editor::Update()
{
	// ENABLE/DISABLE SELECTOR TOOL
	static bool Selector;
	ImGui::Checkbox("Selector", &Selector);


	//RESOURCE MAP INCLUDES
	ImGui::Separator();
	ImGui::Text("Resources To Include in Level");

	for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		const std::string& name = it->first;
		auto& dir = it->second;
		if (ImGui::TreeNode(name.c_str()))
		{
			for (int i = 0; i < dir.size(); ++i)
			{
				ImGui::Checkbox(dir[i].Name.c_str(), &dir[i].Include);
			}
			ImGui::TreePop();
		}
	}
}