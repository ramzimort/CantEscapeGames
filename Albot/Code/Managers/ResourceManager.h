#pragma once
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Material.h"

class DXRenderer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


	void StoreModel(Model* model, const std::string& model_key_name);

	Model* GetModel(const std::string& model_path, bool textured_model = false);

	void StoreMaterial(const Material& material, const std::string& materialName);
	Material* GetMaterial(const std::string& materialName);

	void SetDXRenderer(DXRenderer* dxrenderer);
private:
	std::unordered_map<std::string, Texture*> m_textures;
	//3D meshes models
	std::unordered_map<std::string, Model*> m_models;

	std::unordered_map<std::string, Material> m_materials;

	DXRenderer* m_dxrenderer;
};

