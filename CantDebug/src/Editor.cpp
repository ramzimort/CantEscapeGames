
#include "Editor.h"
#include "..\imgui\imgui.h"

Editor::Editor() { }

Editor::~Editor() { }

void Editor::Update()
{
	static bool Selector;
	ImGui::Checkbox("Selector", &Selector);

	//for (auto it = m_memoryTable.begin(); it != m_memoryTable.end(); ++it)
	//{
	//	if (ImGui::TreeNode(it->first))
	//	{
	//		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	//		{
	//			ImGui::Text("address: %x", *it2);
	//		}
	//		ImGui::TreePop();
	//	}
	//}
}