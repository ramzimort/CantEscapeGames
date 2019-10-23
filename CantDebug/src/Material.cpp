
#include "Material.h"
#include "..\imgui\imgui.h"
#include "..\imgui\imgui_stdlib.h"

MaterialMaker::MaterialMaker()
{

}


MaterialMaker::~MaterialMaker()
{

}

void MaterialMaker::Update()
{
	if (ImGui::Button("Generate Material"))
		*m_materialParams.Pressed = true;

	ImGui::InputText("Output Path", m_materialParams.OutputPath);

	ImGui::Separator();
	ImGui::InputText("Diffuse Texture", m_materialParams.DiffuseTexturePath);
	ImGui::InputText("Normal Texture", m_materialParams.NormalTexturePath);
	ImGui::InputText("Height Texture", m_materialParams.HeightTexturePath);
	ImGui::SliderFloat4("Diffuse Color", m_materialParams.DiffuseColor, 0.0, 1.0);
	ImGui::SliderFloat4("Specular Color", m_materialParams.SpecularColor, 0.0, 1.0);

	ImGui::Separator();
	int i = 0;
	for(auto& mat : *m_materialParams.TextureList)
	{
		if (ImGui::Button(mat.c_str()))
			ImGui::SetClipboardText(mat.c_str());
	}
}

void MaterialMaker::UpdateInfo(CantDebugAPI::MaterialInfo info)
{
	m_materialParams = info;
}
