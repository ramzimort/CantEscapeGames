#pragma once


#include "Helper/Hash.h"

class Model;
class Texture;
class Material;
class DXRenderer;

class ResourceManager
{
	friend class Factory;
	typedef std::unordered_map<StringId, void*, StringIdHash> ResourceMap;
public:
	ResourceManager();
	~ResourceManager();
	void Initialize(DXRenderer* dxrenderer, sol::state* pSolState);
	Model* GetModel(StringId modelId);
	Material* GetMaterial(StringId materialId);
	Texture* GetTexture(StringId textureId);
	std::string& GetPrefab(StringId prefabId);
	sol::table& GetScript(StringId scriptId);

private:
	void LoadModel(const std::string& filePath);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);
	void LoadAudio(const std::string& filePath);
	void LoadPrefab(const std::string& filePath);
	void LoadScript(const std::string& filePath);

private:
	ResourceMap m_resources;
	DXRenderer* m_dxrenderer;
	sol::state* m_pSolState;

	//For scripting
	sol::table refHolder;
};

