
#include "ResourceManager.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Material.h"
#include "Graphics/ModelLoader.h"
#include "Graphics/D3D11_Renderer.h"
#include "Reflection/Serialization.h"
#include "CantDebug/CantDebug.h"

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
#ifdef DEVELOPER
	LoadModel("Cube.fbx", false);
#endif // DEVELOPER
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
	std::string path = Constant::ModelsDir + filePath;
	StringId id = StringId(path);
	Model* model = static_cast<Model*>(m_resources[id]);
	if (model == nullptr)
	{
		model = new Model();
		Assimp::Importer importer;
		aiScene const *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
		// | aiProcess_FixInfacingNormals);// | aiProcess_GenNormals );

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			DEBUG_LOG("Couldn't load Model");
			return;
		}
		m_resources[id] = model;
		ModelLoader::LoadModel(model, scene, m_dxrenderer);
	}
}

void ResourceManager::LoadMaterial(const std::string& filePath)
{
	StringId id = StringId(filePath);
	Material* material = static_cast<Material*>(m_resources[id]);
	if (material != nullptr)
		return;

	material = new Material();
	CantReflect::FromJson(filePath, material);
	m_resources[id] = material;
}

// Returns null if surface does not exist
void ResourceManager::LoadTexture(const std::string& filePath)
{
	//TextureLoader::CreateTexture(m_dxrenderer, TextureLoadDesc(filePath));
	//m_resources[CANTID(filePath)] = 
}

void ResourceManager::ClearModel(Model* model)
{
}

void ResourceManager::ClearMaterial(Material * material)
{
}
