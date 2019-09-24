#pragma once
#include <string>


class Texture;

class Material{
public:
	friend class ResourceManager;
	friend class ModelLoader;
	friend class Factory;
public:
	Material();
	~Material();


	void init();

	//Check Shaders/Shading.h
	uint32_t get_shader_material_type_id() const;


	Texture* get_diffuse_texture() const;
	Texture* get_normal_texture()const ;
	Texture* get_height_texture()const;

	Vector4 get_diffuse_color() const;
	Vector4 get_specular_color() const;


	

private:
	
	std::string m_material_name;

	std::string m_diffuse_tex_dir;
	Texture* m_diffuse_texture;

	std::string m_normal_tex_dir;
	Texture* m_normal_texture;

	std::string m_height_tex_dir;
	Texture* m_height_texture;

	Vector4 m_diffuse_color;
	Vector4 m_specular_color;

};

