#pragma once
#include "../Graphics/Model.h"
#include "../Graphics/Texture.h"


class DXRenderer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


	void StoreModel(Model* model, const std::string& model_key_name);

	Model* GetModel(const std::string& model_path, bool textured_model = false);

	void SetDXRenderer(DXRenderer* dxrenderer);
private:
	std::unordered_map<std::string, Texture*> m_textures;
	//3D meshes models
	std::unordered_map<std::string, Model*> m_models;

	DXRenderer* m_dxrenderer;
};

