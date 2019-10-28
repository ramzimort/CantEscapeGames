#pragma once


#include "Helper/Hash.h"
#include "Audio/AudioTypes.h"
#include "AudioManager.h"

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
};

union ResPtr
{
	Model* p_model;
	Texture* p_texture;
	Material* p_material;
	std::string* p_prefab;
	FMOD::Sound* p_sound;
	sol::table* p_solTable;
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


class ResourceManager
{
	friend class Factory;
	typedef std::unordered_map<StringId, Resource, StringIdHash> ResourceMap;

public:
	ResourceManager();
	~ResourceManager();
	void Initialize(DXRenderer* dxrenderer, sol::state* pSolState, AudioManager* pAudioManager);
	Model* GetModel(StringId modelId);
	Material* GetMaterial(StringId materialId);
	Texture* GetTexture(StringId textureId);
	std::string& GetPrefab(StringId prefabId);
	sol::table& GetScript(StringId scriptId);
	bool HasResource(StringId id);
	void FreeResource(StringId id);
	void FreeAll();

private:
	void LoadModel(const std::string& filePath);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);
	void LoadAudio(const std::string& filePath, Category type);
	void LoadPrefab(const std::string& filePath);
	void LoadScript(const std::string& filePath);
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif

private:
	ResourceMap m_resources;
	DXRenderer* m_dxrenderer;
	sol::state* m_pSolState;
	AudioManager* m_pAudioManager;

	//For scripting
	sol::table refHolder;
};

