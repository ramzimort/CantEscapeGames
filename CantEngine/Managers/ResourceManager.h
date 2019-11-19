#pragma once
#include "Helper/Hash.h"
#include "Audio/AudioTypes.h"
#include "AudioManager.h"
#include <DirectXTK/SpriteFont.h>

/**
 * @file ResourceManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Responsible for storing resource pointer table and respective memory management
 */

class Model;
class Texture;
class Material;
class DXRenderer;
class AudioManager;

/**
 * @brief Represents type of resource pointer being stored
 * 
 */
enum ResType
{
	SFX,
	SONGS,
	TEXTURE,
	MODEL,
	MATERIAL,
	PREFAB,
	SCRIPT,
	FONT,
};

/**
 * @brief Union class that can store different pointer types
 * 
 */
union ResPtr
{
	Model* p_model;
	Texture* p_texture;
	Material* p_material;
	std::string* p_prefab;
	FMOD::Sound* p_sound;
	sol::table* p_solTable;
	DirectX::SpriteFont* p_spriteFont;
};

/**
 * @brief Resource table object containing type and pointer
 * 
 */
struct Resource
{
	Resource() = default;
	~Resource() = default;
	/**
	 * @brief Construct a new Resource object containing type and pointer
	 * 
	 * @param t 
	 * @param p 
	 */
	Resource(ResType t, ResPtr p) :
		type(t), res(p) { }
	ResType type;
	ResPtr res;
};

/**
 * @brief  Responsible for storing resource pointer table and respective memory management
 * 
 */
class ResourceManager
{
	friend class Factory;
	typedef std::unordered_map<StringId, Resource, StringIdHash> ResourceMap;
	typedef std::unordered_map<const std::string, const std::wstring, std::hash<std::string>> LocStringMap;
	friend class ModelLoader;
public:
	ResourceManager();
	~ResourceManager();
	/**
	 * @brief Initialize resource related managers
	 * 
	 * @param dxrenderer 
	 * @param pSolState 
	 * @param pAudioManager 
	 */
	void Initialize(DXRenderer* dxrenderer, sol::state* pSolState, AudioManager* pAudioManager);
	/**
	 * @brief Get Localized String from table
	 * 
	 * @return const std::wstring& 
	 */
	const std::wstring& GetLString(const std::string&);
	/**
	 * @brief Get the Model object
	 * 
	 * @param modelId 
	 * @return Model* 
	 */
	Model* GetModel(StringId modelId);
	/**
	 * @brief Get the Material object
	 * 
	 * @param materialId 
	 * @return Material* 
	 */
	Material* GetMaterial(StringId materialId);
	/**
	 * @brief Get the Texture object
	 * 
	 * @param textureId 
	 * @return Texture* 
	 */
	Texture* GetTexture(StringId textureId);
	/**
	 * @brief Get the Font object
	 * 
	 * @param fontId 
	 * @return DirectX::SpriteFont* 
	 */
	DirectX::SpriteFont* GetFont(StringId fontId);
	/**
	 * @brief Get the Prefab string (JSON format)
	 * 
	 * @param prefabId 
	 * @return std::string& 
	 */
	std::string& GetPrefab(StringId prefabId);
	/**
	 * @brief Get the LUA Script object file
	 * 
	 * @param scriptId 
	 * @return sol::table& 
	 */
	sol::table& GetScript(StringId scriptId);
	/**
	 * @brief Check if resource with given ID (path) has been loaded
	 * 
	 * @param id 
	 * @return true 
	 * @return false 
	 */
	bool HasResource(StringId id);
	/**
	 * @brief Free Resource with given ID if it exists
	 * 
	 * @param id 
	 */
	void FreeResource(StringId id);
	/**
	 * @brief Free All loaded resources
	 * 
	 */
	void FreeAll();

private:
	void LoadFont(const std::string& filePath);
	void LoadStrings(const std::string& language);
	void LoadModel(const std::string& filePath);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);
	void LoadAudio(const std::string& filePath, Category type);
	void LoadPrefab(const std::string& filePath);
	void LoadScript(const std::string& filePath);
	void LoadInternalCompressedTexture(const std::string& keyName, 
		const unsigned char* rawData, uint32_t totalByteWidth);
	void StoreMaterial(const std::string& keyName, Material* pMaterial);
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
	void ReloadResources();
#endif

private:
	ResourceMap m_resources;
	LocStringMap m_lStrings;

	DXRenderer* m_dxrenderer;
	sol::state* m_pSolState;
	AudioManager* m_pAudioManager;

	//For scripting
	sol::table refHolder;
};

