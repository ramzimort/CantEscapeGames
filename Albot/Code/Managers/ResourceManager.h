#pragma once


#include "Helper/Hash.h"

class Model;
class Texture;
class Material;
class DXRenderer;

class ResourceManager
{
	typedef std::unordered_map<StringId, void*, StringIdHash> ResourceMap;
public:
	ResourceManager();
	~ResourceManager();
	void SetDXRenderer(DXRenderer* dxrenderer);
	Model* GetModel(StringId modelId);
	Material* GetMaterial(StringId materialId);
	Texture* GetTexture(StringId textureId);

	void LoadModel(const std::string& filePath, bool texturedModel = false);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);

	void ClearModel(Model* model);
	void ClearMaterial(Material* material);

private:
	ResourceMap m_resources;
	DXRenderer* m_dxrenderer;
};

