#include "Material.h"
#include "../Shaders/Shading.h"


Material::Material()
	:m_material_name(""),
	m_diffuse_color(),
	m_diffuse_tex_dir(""),
	m_normal_tex_dir(""),
	m_diffuse_texture(nullptr),
	m_normal_texture(nullptr),
	m_height_texture(nullptr)
{
}


Material::~Material()
{
}


void Material::init()
{

}


uint32_t Material::get_shader_material_type_id() const
{
	if (m_height_texture && m_diffuse_texture && m_normal_texture)
	{
		return MAT_ID_DIFFUSE_NORMAL_PARALLAX_TEXTURE;
	}
	else if (m_diffuse_texture && m_normal_texture)
	{
		return MAT_ID_DIFFUSE_NORMAL_TEXTURE;
	}
	else if (m_diffuse_texture)
	{
		return MAT_ID_DIFFUSE_TEXTURE;
	}

	return MAT_ID_COLOR;
}

Texture* Material::get_diffuse_texture() const
{
	return m_diffuse_texture;
}

Texture* Material::get_normal_texture() const
{
	return m_normal_texture;
}

Texture* Material::get_height_texture() const
{
	return m_height_texture;
}

Vector4 Material::get_diffuse_color() const
{
	return m_diffuse_color;
}

Vector4 Material::get_specular_color() const
{
	return m_specular_color;
}