
#include "Editor.h"
#include "Directory.h"
#include "..\imgui\imgui.h"

using namespace std;

Editor::Editor()
{ 
}

Editor::~Editor()
{ }

void Editor::Clear()
{
	m_resourceMap.clear();
}

void Editor::UpdateObjects(const char* id, const char* name, bool* pClicked, bool* pDoubleClicked, bool created)
{
	Info info; info.Name = name; info.Pressed = pClicked; info.DoubleClicked = pDoubleClicked; info.ID = id;
	if (created)
		m_objects.push_back(info);
	else
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (!strcmp(it->Name.c_str(), name))
			{
				m_objects.erase(it);
				return;
			}
		}
	}
}

void Editor::UpdateResources(const char* dir, const char* asset, bool* pFlag)
{
	Info info; info.Name = asset; info.Pressed = pFlag;
	string dirName(dir);
	auto it = m_resourceMap.find(dirName);

	if (m_resourceMap.find(dirName) != m_resourceMap.end())
	{
		m_resourceMap.insert(std::make_pair(dirName, vector<Info>()));
	}
	m_resourceMap[dirName].push_back(info);

}

void Editor::UpdatePrefabs(const char* prefabName, bool* p_buttonState)
{
	Info info; info.Pressed = p_buttonState; info.Name = prefabName;
	m_prefabButtons.push_back(info);
}

void Editor::UpdateSettings(const char* checkboxName, bool* pFlag)
{
	m_queue.m_queueData.push(CheckboxQueue::CheckboxData{ checkboxName, pFlag });
}

void Editor::Update()
{
	// SETTINGS
	ImGui::Text("Editor Settings");
	while (!m_queue.m_queueData.empty())
	{
		const CheckboxQueue::CheckboxData& data = m_queue.m_queueData.front();
		ImGui::Checkbox(data.m_checkBoxName.c_str(), data.m_pFlag);
		m_queue.m_queueData.pop();
	}


	// Objects
	ImGui::Separator();
	ImGui::Text("Objects");
	for (auto& object : m_objects)
	{
		ImGui::Checkbox(object.ID.c_str(), object.Pressed);
		ImGui::SameLine(); 	
		if(ImGui::Button(object.Name.c_str()))
			*(object.DoubleClicked) = true;			
	}
	
	// Resources
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
				ImGui::Checkbox(dir[i].Name.c_str(), dir[i].Pressed);
			}
			ImGui::TreePop();
		}
	}

	// ===================

	ImGui::Separator();
	ImGui::Text("Prefabs To Load");
	int a = 5;
	for (auto& prefab : m_prefabButtons)
	{
		if (ImGui::Button(prefab.Name.c_str()))
		{
			*(prefab.Pressed) = true;
		}
	}

}