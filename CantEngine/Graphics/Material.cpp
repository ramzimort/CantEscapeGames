#include "Material.h"
#include "../Shaders/Shading.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Material>("Material")
		.property("Diffuse_Color", &Material::m_diffuseColor)
		.property("Specular_Color", &Material::m_specularColor)
		.property("Diffuse_Texture_Dir", &Material::m_diffuseTextureId)
		.property("Normal_Texture_Dir", &Material::m_normalTextureId)
		.property("Height_Texture_Dir", &Material::m_heightTextureId)
		.property("Specular_Texture_Dir", &Material::m_specularTextureId)
		.property("Metallic_Texture_Dir", &Material::m_metallicTextureId)
		.property("Roughness_Texture_Dir", &Material::m_roughnessTextureId)
		.property("Metallic_Value", &Material::m_metallicValue)
		.property("Roughness_Value", &Material::m_roughnessValue)
		.method("Init", &Material::Init);
}

Material::Material() :
	m_pDiffuseTexture(nullptr),
	m_pNormalTexture(nullptr),
	m_pHeightTexture(nullptr),
	m_pSpecularTexture(nullptr),
	m_pRoughnessTexture(nullptr),
	m_pMetallicTexture(nullptr),
	m_materialId(""),
	m_diffuseTextureId(""),
	m_normalTextureId(""),
	m_heightTextureId(""),
	m_specularTextureId(""),
	m_roughnessTextureId(""),
	m_metallicTextureId(""),
	m_metallicValue(0.1f),
	m_roughnessValue(0.5f)
{
}


Material::~Material()
{
}


void Material::Init()
{

}

uint32_t Material::GetShaderMaterialType() const
{
	uint32_t materialID = (uint32_t)MAT_ID_COLOR;
	if (m_pHeightTexture)
	{
		materialID |= (uint32_t)MAT_ID_PARALLAX_TEXTURE;
	}
	if (m_pNormalTexture)
	{
		materialID |= (uint32_t)MAT_ID_NORMAL_TEXTURE;
	}
	if (m_pDiffuseTexture)
	{
		materialID |= (uint32_t)MAT_ID_DIFFUSE_TEXTURE;
	}
	if (m_pSpecularTexture)
	{
		materialID |= (uint32_t)MAT_ID_SPECULAR_TEXUTERE;
	}
	if (m_pRoughnessTexture)
	{
		materialID |= (uint32_t)MAT_ID_ROUGHNESS_TEXTURE;
	}
	if (m_pMetallicTexture)
	{
		materialID |= (uint32_t)MAT_ID_METALLIC_TEXTURE;
	}
	return materialID;
}

Texture* Material::GetDiffuseTexture() const
{
	return m_pDiffuseTexture;
}

Texture* Material::GetNormalTexture() const
{
	return m_pNormalTexture;
}

Texture* Material::GetHeightTexture() const
{
	return m_pHeightTexture;
}

Texture* Material::GetSpecularTexture() const
{
	return m_pSpecularTexture;
}

Texture* Material::GetMetallicTexture() const
{
	return m_pMetallicTexture;
}

Texture* Material::GetRoughnessTexture() const
{
	return m_pRoughnessTexture;
}

Vector4 Material::GetDiffuseColor() const
{
	return m_diffuseColor;
}

Vector4 Material::GetSpecularColor() const
{
	return m_specularColor;
}

float Material::GetMetallicValue() const
{
	return m_metallicValue;
}
float Material::GetRoughnessValue() const
{
	return m_roughnessValue;
}