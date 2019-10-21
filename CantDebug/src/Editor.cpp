
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

Editor::Editor()
{ 
}

Editor::~Editor()
{ }

void Editor::Clear()
{
	m_resourceMap.clear();
}

void Editor::UpdateResources(const char* dir, const char* asset, bool* pFlag)
{
	Info info; info.Name = asset; info.Include = pFlag;
	string dirName(dir);
	auto it = m_resourceMap.find(dirName);

	if (m_resourceMap.find(dirName) != m_resourceMap.end())
	{
		m_resourceMap.insert(std::make_pair(dirName, vector<Info>()));
	}
	m_resourceMap[dirName].push_back(info);

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
				ImGui::Checkbox(dir[i].Name.c_str(), dir[i].Include);
				ImGui::SameLine();  ImGui::Button(dir[i].Name.c_str());
			}
			ImGui::TreePop();
		}
	}

	// ===================


}