
#include "Editor.h"
#include "Directory.h"
#include "..\imgui\imgui.h"
#include "..\imgui\imgui_stdlib.h"

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

void Editor::UpdateComponents(PropertyInfo info)
{
	if (m_components.find(info.goName) == m_components.end())
		m_components.insert(std::make_pair(info.goName, std::map<std::string, std::list<PropertyInfo>>()));
	//if (m_components.at(info.goName).find(info.compName) == m_components.at(info.goName).end())
	
	auto& comps = m_components.at(info.goName);
	if (comps.find(info.compName) == comps.end())
		comps.insert(std::make_pair(info.compName, std::list<PropertyInfo>()));
	
	comps.at(info.compName).push_back(info);
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
	bool editing = false;
	for (auto& object : m_objects)
	{
		ImGui::Checkbox(object.ID.c_str(), object.Pressed);
		ImGui::SameLine(); 	
		if(ImGui::Button(object.Name.c_str()))
			*(object.DoubleClicked) = true;			
		if (*object.Pressed && !editing)
		{
			UpdateComponentWindow();
			editing = true;
		}
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

void Editor::UpdateComponentWindow()
{
	// Show editor window
	ImGuiIO& io = ImGui::GetIO();
	const float MARGIN = 2.0f;
	const float MAXWIDTH = io.DisplaySize.x;
	ImVec2 window_pos = ImVec2(MAXWIDTH-400, MARGIN);
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, io.DisplaySize.y), ImVec2(400, io.DisplaySize.y));
	if (ImGui::Begin("Object Editor"), 0, ImGuiWindowFlags_NoCollapse)
	{
		// Place components here to be used by the thing.
		int i = 0;
		ImGui::Separator();
		ImGui::Separator();

		for(auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			const std::string& objName = it->first;
			auto& comps = it->second;
			// Obj Tree
			i++; ImGui::PushID(i);
			

			ImGui::Text(objName.c_str());
			ImGui::Separator();
			for (auto it2 = comps.begin(); it2 != comps.end(); ++it2)
			{
				ImGui::Text(it2->first.c_str());
				auto& props = it2->second;
				for (auto it3 = props.begin(); it3 != props.end(); ++it3)
				{
					ImGui::Indent(10.f);
					switch (it3->t)
					{
					case CantDebugAPI::FLOAT:
						ImGui::SliderFloat(it3->propName.c_str(), it3->f, it3->min, it3->max);
						break;
					case VEC2:
						ImGui::SliderFloat2(it3->propName.c_str(), it3->f, it3->min, it3->max);
						break;
					case VEC3:
						ImGui::SliderFloat3(it3->propName.c_str(), it3->f, it3->min, it3->max);
						break;
					case VEC4:
						ImGui::SliderFloat4(it3->propName.c_str(), it3->f, it3->min, it3->max);
						break;
					case INTEGER:
						ImGui::SliderInt(it3->propName.c_str(), (int*)(it3->i), static_cast<int>(it3->min), static_cast<int>(it3->max));
						break;
					case STRING:
						ImGui::InputText(it3->propName.c_str(), it3->propValString);
						break;
					}
					ImGui::Indent(-10.f);
				}
				ImGui::Separator();
			}
			ImGui::Separator();
			ImGui::Separator();
			ImGui::PopID();
		}
		m_components.clear();
	}
	ImGui::End();
}