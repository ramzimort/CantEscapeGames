#pragma once


#include "Helper/Hash.h"
#include "Audio/AudioTypes.h"
#include "AudioManager.h"
#include <DirectXTK/SpriteFont.h>

class Model;
class Texture;
class Material;
class DXRenderer;
class AudioManager;


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

struct Resource
{
	Resource() = default;
	~Resource() = default;
	Resource(ResType t, ResPtr p) :
		type(t), res(p) { }
	ResType type;
	ResPtr res;
};

//template <>
//struct std::hash<std::wstring>
//{
//	std::size_t operator()(const std::wstring& k) const
//	{
//		using std::size_t;
//		using std::hash;
//		using std::wstring;
//
//		// Compute individual hash values for first,
//		// second and third and combine them using XOR
//		// and bit shifting:
//
//		return ((hash<wstring>()(k)
//	}
//};


class ResourceManager
{
	friend class Factory;
	typedef std::unordered_map<StringId, Resource, StringIdHash> ResourceMap;
	typedef std::unordered_map<const std::string, const std::wstring, std::hash<std::string>> LocStringMap;
	friend class ModelLoader;
public:
	ResourceManager();
	~ResourceManager();

	void Initialize(DXRenderer* dxrenderer, sol::state* pSolState, AudioManager* pAudioManager);
	const std::wstring& GetLString(const std::string&);
	Model* GetModel(StringId modelId);
	Material* GetMaterial(StringId materialId);
	Texture* GetTexture(StringId textureId);
	DirectX::SpriteFont* GetFont(StringId fontId);
	std::string& GetPrefab(StringId prefabId);
	sol::table& GetScript(StringId scriptId);
	bool HasResource(StringId id);
	void FreeResource(StringId id);
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

