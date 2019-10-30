
#include "ResourceManager.h"
#include "AudioManager.h"
#include "Animation/AnimModel.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Material.h"
#include "Graphics/Models/ModelLoader.h"
#include "Animation/FBXLoader.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/DXResourceLoader.h"
#include "Reflection/Serialization.h"
#include "CantDebug/CantDebug.h"
#include "Memory/CantMemory.h"

ResourceManager::ResourceManager() : m_dxrenderer(nullptr)
{
	m_lStrings.clear();
	m_lStrings.insert(std::make_pair("", L"___"));
}

ResourceManager::~ResourceManager()
{
	FreeAll();
	m_lStrings.clear();
}

void ResourceManager::Initialize(DXRenderer* dxrenderer, sol::state* pSolState, AudioManager* pAudioManager)
{
	m_dxrenderer = dxrenderer;
	m_pSolState = pSolState;
	m_pAudioManager = pAudioManager;
}

const std::wstring& ResourceManager::GetLString(const std::string& stringId)
{
	const auto& it = m_lStrings.find(stringId);
	if (it == m_lStrings.end())
		return m_lStrings[""];
	return it->second;
}

void ResourceManager::LoadStrings(const std::string& language)
{
	std::wifstream in_file;
	in_file.open(language);
	if (!in_file.is_open())
	{
		DEBUG_LOG("Couldn't Load File %s", language.c_str());
		return;
	}

	std::wstring line;
	std::wstring Lid, keyword, lstring;
	std::string key;
	size_t id = 0;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	while (std::getline(in_file, line)) // read whole line into line
	{
		std::wistringstream iss(line); // string stream
		std::getline(iss, Lid, L','); // read first part up to comma, ignore the comma
		std::getline(iss, keyword, L','); // read first part up to comma, ignore the comma
		key = converter.to_bytes(keyword);

		std::getline(iss, lstring, L'\n'); // read first part up to comma, ignore the comma
		//auto it = m_lStrings.find(key);
		//if (it != m_lStrings.end())
		//	it = m_lStrings.erase(it);
		m_lStrings.insert(std::make_pair(key, lstring));
		++id;

		DEBUG_LOG("Key: %s, Value: %s\n", key.c_str(), lstring.c_str());
	}
}



Model* ResourceManager::GetModel(StringId modelId)
{
	return m_resources.at(modelId).res.p_model;
}

Material* ResourceManager::GetMaterial(StringId materialId)
{
	return m_resources.at(materialId).res.p_material;

}

Texture* ResourceManager::GetTexture(StringId textureId)
{
	return m_resources.at(textureId).res.p_texture;
}

std::string& ResourceManager::GetPrefab(StringId prefabId)
{
	return *m_resources.at(prefabId).res.p_prefab;
}

sol::table& ResourceManager::GetScript(StringId scriptId)
{
	//return *(static_cast<sol::table*>(m_resources[scriptId]));

	auto node = m_resources.find(scriptId);
	if (node == m_resources.end())
		return refHolder;
	else
		return *node->second.res.p_solTable;
}


void ResourceManager::LoadModel(const std::string& filePath)
{
	DEBUG_LOG("Loading Model: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;

	Model* model = nullptr;
	Assimp::Importer importer;
	aiScene const *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
	// | aiProcess_FixInfacingNormals);// | aiProcess_GenNormals );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("Couldn't load Model %s", filePath.c_str());
		return;
	}

	//Figure out if we need animModel or not based on bones
	bool hasBoneFlag = false;
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		if (scene->mMeshes[i]->HasBones())
		{
			hasBoneFlag = true;
			break;
		}
	}

	//Create model with the right virtual ctor
	model = (hasBoneFlag) ? CantMemory::PoolResource<AnimModel>::Allocate() : CantMemory::PoolResource<Model>::Allocate();

	//First load mesh data. If has bones, load extra bones parameters
	ModelLoader::LoadModel(model, scene, m_dxrenderer);
	if (hasBoneFlag)
		FBXLoader::LoadSkeletalData(model, scene);

	//Finally, init buffer for renderer
	model->InitBuffer(m_dxrenderer);

	ResPtr p; 
	p.p_model = model;
	Resource res(MODEL, p);
	m_resources[id] = res;
}


void ResourceManager::LoadMaterial(const std::string& filePath)
{
	DEBUG_LOG("Loading Material: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;

	const std::string materialObj = CantReflect::StringifyJson(filePath);
	Material* material = CantMemory::PoolResource<Material>::Allocate();
	CantReflect::FromJson(materialObj, material);

	if (!(material->m_diffuseTextureId == ""))
	{
		material->m_pDiffuseTexture = GetTexture(material->m_diffuseTextureId);
	}

	if ( !(material->m_normalTextureId == "") )
	{
		material->m_pNormalTexture = GetTexture(material->m_normalTextureId);
	}

	if (!(material->m_heightTextureId == ""))
	{
		material->m_pHeightTexture = GetTexture(material->m_heightTextureId);
	}

	ResPtr p; p.p_material = material;
	Resource res(MATERIAL, p);
	m_resources[id] = res;
}

bool ResourceManager::HasResource(StringId id)
{
	return m_resources.find(id) != m_resources.end();
}


void ResourceManager::FreeResource(StringId id)
{
	auto it = m_resources.find(id);
	if (it == m_resources.end())
		return;
	ResPtr p = it->second.res;
	switch (it->second.type)
	{
	case SONGS:
	case SFX:
		p.p_sound->release();
		m_pAudioManager->UnregisterSound(it->first);
		break;
	case TEXTURE:
	{
		p.p_texture->Release();
		CantMemory::PoolResource<Texture>::Free(p.p_texture);
		break;
	}
	case MODEL:
	{
		p.p_model->Release();
		CantMemory::PoolResource<Model>::Free(p.p_model);
		break;
	}
	case MATERIAL:
		CantMemory::PoolResource<Material>::Free(p.p_material);
		break;
	case PREFAB:
		CantMemory::PoolResource<std::string>::Free(p.p_prefab);
		break;
	case SCRIPT:
		// TODO: Should we do anything when clearing this type of resource
		CantMemory::PoolResource<sol::table>::Free(p.p_solTable);
		break;
	}
	m_resources.erase(it);
}

void ResourceManager::FreeAll()
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		FreeResource(it->first);
	}
	m_resources.clear();
}

// Returns null if surface does not exist
void ResourceManager::LoadTexture(const std::string& filePath)
{
	std::lock_guard<std::mutex> lock(m_dxrenderer->m_mutex);
	DEBUG_LOG("Loading Texture: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;

	TextureLoadDesc loadTextureDesc = {};
	loadTextureDesc.m_file_name = filePath;
	loadTextureDesc.m_rawData = nullptr;

	Texture* texture = DXResourceLoader::Create_Texture(m_dxrenderer, loadTextureDesc);

	ResPtr p; p.p_texture = texture;
	Resource res(TEXTURE, p);
	m_resources[id] = res;
}

void ResourceManager::LoadAudio(const std::string& filePath, Category type)
{
	DEBUG_LOG("Loading Audio: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;
	ResPtr p;
	switch (type)
	{
	case Category::CATEGORY_SFX:
		p.p_sound = nullptr;
		m_resources.insert(std::make_pair(id, Resource(SFX, p)));
		break;
	case Category::CATEGORY_SONG:
		p.p_sound = nullptr;
		m_resources.insert(std::make_pair(id, Resource(SONGS, p)));		
		break;
	}
	m_pAudioManager->mp_system->createSound(filePath.c_str(), m_pAudioManager->m_modes[type], 0, &m_resources[id].res.p_sound);
	m_pAudioManager->RegisterSound(SoundData(id, m_resources[id].res.p_sound, type));
}

void ResourceManager::LoadPrefab(const std::string& filePath)
{
	DEBUG_LOG("Loading Prefab: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;
	
	std::string* defaultGameObj = CantMemory::PoolResource<std::string>::Allocate(CantReflect::StringifyJson(filePath));

	ResPtr p; p.p_prefab = defaultGameObj;
	Resource res(PREFAB, p);
	m_resources[id] = res;
}

void ResourceManager::LoadScript(const std::string& filePath)
{
	DEBUG_LOG("Loading Script: %s...\n", filePath.c_str());
	StringId id = StringId(filePath);

	if (m_resources.find(id) != m_resources.end())
		return;

	sol::table* pLuaTable = CantMemory::PoolResource<sol::table>::Allocate();

	if (*pLuaTable == sol::lua_nil)
	{
		try
		{
			//Load the script and retrieve the table
			*pLuaTable = m_pSolState->script_file(filePath);

			ResPtr p; p.p_solTable = pLuaTable;
			Resource res(SCRIPT, p);
			m_resources[id] = res;
		}
		catch (const sol::error& e)
		{
			const char *errorName = e.what();
			DEBUG_LOG(errorName);
			assert(0);
		}
	}
}

#ifdef  DEVELOPER
void ResourceManager::ReloadResources()
{
	auto it = m_resources.begin();
	while (it != m_resources.end())
	{
		const std::string& path = it->first.getName();
		auto p = it->second.res;
		switch (it->second.type)
		{		
		//case SONGS:
		//	p.p_sound->release();
		//	m_pAudioManager->UnregisterSound(path);
		//	p.p_sound = nullptr;
		//	m_pAudioManager->mp_system->createSound(path.c_str(), CATEGORY_SONG, 0, &p.p_sound);
		//	m_pAudioManager->RegisterSound(SoundData(it->first, p.p_sound, Category::CATEGORY_SONG));
		//	break;
		//case SFX:
		//	p.p_sound->release();
		//	m_pAudioManager->UnregisterSound(path);
		//	p.p_sound = nullptr;
		//	m_resources.insert(std::make_pair(it->first, Resource(SFX, p)));
		//	m_pAudioManager->mp_system->createSound(path.c_str(), CATEGORY_SFX, 0, &p.p_sound);
		//	m_pAudioManager->RegisterSound(SoundData(it->first, p.p_sound, Category::CATEGORY_SFX));
		//	break;
		/*case TEXTURE:
		{
			p.p_texture->Release();
			CantMemory::PoolResource<Texture>::Free(p.p_texture);
			LoadTexture(path);
			break;
		}*/
		case MODEL:
		{
			//p.p_model->Release();
			//CantMemory::PoolResource<Model>::Free(p.p_model);

			//Model* model = nullptr;
			//Assimp::Importer importer;
			//aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
			//// | aiProcess_FixInfacingNormals);// | aiProcess_GenNormals );

			//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			//{
			//	DEBUG_LOG("Couldn't load Model %s", path.c_str());
			//	return;
			//}

			////Figure out if we need animModel or not based on bones
			//bool hasBoneFlag = false;
			//for (unsigned i = 0; i < scene->mNumMeshes; ++i)
			//{
			//	if (scene->mMeshes[i]->HasBones())
			//	{
			//		hasBoneFlag = true;
			//		break;
			//	}
			//}

			////Create model with the right virtual ctor
			//model = (hasBoneFlag) ? CantMemory::PoolResource<AnimModel>::Allocate() : CantMemory::PoolResource<Model>::Allocate();

			////First load mesh data. If has bones, load extra bones parameters
			//ModelLoader::LoadModel(model, scene, m_dxrenderer);
			//if (hasBoneFlag)
			//	FBXLoader::LoadSkeletalData(model, scene);

			////Finally, init buffer for renderer
			//model->InitBuffer(m_dxrenderer);
			//break;
		}
		case MATERIAL:
		{
			CantMemory::PoolResource<Material>::Free(p.p_material);
			Material* material = CantMemory::PoolResource<Material>::Allocate();
			const std::string materialObj = CantReflect::StringifyJson(path);
			CantReflect::FromJson(materialObj, material);

			if (!(material->m_diffuseTextureId == ""))
			{
				material->m_pDiffuseTexture = GetTexture(material->m_diffuseTextureId);
			}

			if (!(material->m_normalTextureId == ""))
			{
				material->m_pNormalTexture = GetTexture(material->m_normalTextureId);
			}

			if (!(material->m_heightTextureId == ""))
			{
				material->m_pHeightTexture = GetTexture(material->m_heightTextureId);
			}
			p.p_material = material;
			break;
		}
		case PREFAB:
		{
			CantMemory::PoolResource<std::string>::Free(p.p_prefab);
			std::string* defaultGameObj = CantMemory::PoolResource<std::string>::Allocate(CantReflect::StringifyJson(path));
			ResPtr p; p.p_prefab = defaultGameObj;
			break;
		}
		case SCRIPT:
		{
			// TODO: Should we do anything when clearing this type of resource
			CantMemory::PoolResource<sol::table>::Free(p.p_solTable);
			sol::table* pLuaTable = CantMemory::PoolResource<sol::table>::Allocate();

			if (*pLuaTable == sol::lua_nil)
			{
				try
				{
					//Load the script and retrieve the table
					*pLuaTable = m_pSolState->script_file(path);
					p.p_solTable = pLuaTable;
				}
				catch (const sol::error & e)
				{
					const char* errorName = e.what();
					DEBUG_LOG(errorName);
					assert(0);
				}
			}
			break;
		}
		}
		++it;
	}
}
#endif //  DEVELOPER