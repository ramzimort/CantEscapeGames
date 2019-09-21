#include "ResourceManager.h"
#include "../Graphics/ModelLoader.h"
#include "../Graphics/D3D11_Renderer.h"

ResourceManager::ResourceManager()
	:m_dxrenderer(nullptr)
{
}


ResourceManager::~ResourceManager()
{
}




void ResourceManager::StoreModel(Model* model, const std::string& model_key_name)
{
	m_models[model_key_name] = model;
}

Model* ResourceManager::GetModel(const std::string& modelPath, bool texturedModel)
{
	Model *model = m_models[modelPath];

	if (model == nullptr)
	{
		model = new Model();
		ModelLoader::LoadModel(m_dxrenderer, Constant::ModelsDir + modelPath, *model, texturedModel);
		m_models[modelPath] = model;
	}
	return model;
}

void ResourceManager::SetDXRenderer(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;
}