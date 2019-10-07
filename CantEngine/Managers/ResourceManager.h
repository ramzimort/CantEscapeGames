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
	void SetDXRenderer(DXRenderer* dxrenderer);
	Model* GetModel(StringId modelId);
	Material* GetMaterial(StringId materialId);
	Texture* GetTexture(StringId textureId);
	std::string& GetPrefab(StringId prefabId);

private:
	void LoadModel(const std::string& filePath);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);
	void LoadAudio(const std::string& filePath);
	void LoadPrefab(const std::string& filePath);

private:
	ResourceMap m_resources;
	DXRenderer* m_dxrenderer;
};

