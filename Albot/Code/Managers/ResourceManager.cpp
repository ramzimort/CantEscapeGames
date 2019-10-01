
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
	LoadModel("Assets/Models/Cube.fbx");
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

std::string& ResourceManager::GetPrefab(StringId prefabId)
{
	return *(static_cast<std::string*>(m_resources[prefabId]));
}

void ResourceManager::LoadModel(const std::string& filePath)
{
	DEBUG_LOG("Loading Model: %s...\n", filePath);
	StringId id = StringId(filePath);

	Model* model = static_cast<Model*>(m_resources[id]);
	if (model != nullptr)
		return;

	model = new Model();
	Assimp::Importer importer;
	aiScene const *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
	// | aiProcess_FixInfacingNormals);// | aiProcess_GenNormals );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("Couldn't load Model %s", filePath);
		return;
	}
	m_resources[id] = model;
	ModelLoader::LoadModel(model, scene, m_dxrenderer);
}

void ResourceManager::LoadMaterial(const std::string& filePath)
{
	DEBUG_LOG("Loading Material: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	Material* material = static_cast<Material*>(m_resources[id]);
	if (material != nullptr)
		return;

	const std::string materialObj = CantReflect::StringifyJson(filePath);
	material = new Material();
	CantReflect::FromJson(materialObj, material);
	m_resources[id] = material;
}

// Returns null if surface does not exist
void ResourceManager::LoadTexture(const std::string& filePath)
{
	DEBUG_LOG("Loading Texture: %s...\n", filePath);
	StringId id = StringId(filePath);

	Texture* texture = static_cast<Texture*>(m_resources[id]);
	if (texture != nullptr)
		return;

	//TextureLoader::CreateTexture(m_dxrenderer, TextureLoadDesc(filePath));
	//m_resources[CANTID(filePath)] = 
}

void ResourceManager::LoadAudio(const std::string& filePath)
{
	DEBUG_LOG("Loading Audio: %s...\n", filePath);
	StringId id = StringId(filePath);
}

void ResourceManager::LoadPrefab(const std::string& filePath)
{
	DEBUG_LOG("Loading Prefab: %s...\n", filePath);
	StringId id = StringId(filePath);

	std::string* defaultGameObj = static_cast<std::string*>(m_resources[id]);
	if (defaultGameObj != nullptr)
		return;
	
	defaultGameObj = new std::string(CantReflect::StringifyJson(filePath));
	m_resources[id] = defaultGameObj;
}
