#include "ResourceManager.h"
#include "../Graphics/ModelLoader.h"
#include "../Graphics/D3D11_Renderer.h"

ResourceManager::ResourceManager() : 
	m_dxrenderer(nullptr)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::SetDXRenderer(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;
}

Model* ResourceManager::GetModel(StringId modelId)
{
	return static_cast<Model*>(m_resources[modelId]);
}

Material* ResourceManager::GetMaterial(StringId materialId)
{
	return static_cast<Material*>(m_resources[materialId]);
}

Texture* ResourceManager::GetTexture(StringId textureId)
{
	return static_cast<Texture*>(m_resources[textureId]);
}

void ResourceManager::LoadModel(const std::string& filePath, bool texturedModel)
{
	StringId id(filePath);
	Model* model = static_cast<Model*>(m_resources[id]);
	if (model == nullptr)
		m_resources[id] = ModelLoader::LoadModel(m_dxrenderer, filePath, texturedModel);
}

void ResourceManager::LoadMaterial(const std::string& filePath)
{

}

// Returns null if surface does not exist
void ResourceManager::LoadTexture(const std::string& filePath)
{

}