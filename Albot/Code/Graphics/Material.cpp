#include "Material.h"
#include "../Shaders/Shading.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Material>("Material")
		.property("Model", &Material::m_diffuseColor)
		.property("Name", &Material::m_specularColor)
		.method("Init", &Material::Init);
}

Material::Material() :
	m_materialName(""),
	m_diffuseColor(),
	m_diffuseTextureDirectory(""),
	m_normalTextureDirectory(""),
	m_pDiffuseTexture(nullptr),
	m_pNormalTexture(nullptr),
	m_pHeightTexture(nullptr)
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
	if (m_pDiffuseTexture && m_pNormalTexture)
	{
		materialID |= (uint32_t)MAT_ID_NORMAL_TEXTURE;
	}
	if (m_pDiffuseTexture)
	{
		materialID |= (uint32_t)MAT_ID_DIFFUSE_TEXTURE;
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

Vector4 Material::GetDiffuseColor() const
{
	return m_diffuseColor;
}

Vector4 Material::GetSpecularColor() const
{
	return m_specularColor;
}