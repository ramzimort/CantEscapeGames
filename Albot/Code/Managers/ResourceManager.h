#pragma once

#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Material.h"

class DXRenderer;

class StringId
{
public:
	StringId() = default;
	StringId(const std::string& _name) : 
#ifndef DEVELOPER
		m_name(_name),
#endif
		m_sid(std::hash<std::string>{ }(_name)) { }

	bool operator==(const StringId& rhs) const { return m_sid == rhs.m_sid; }
#ifndef DEVELOPER
	const std::string& m_name;
#endif
	size_t m_sid;
};

struct StringIdHash
{
	size_t operator()(const StringId& sid) const { return sid.m_sid; }
};

#define SID(ANY_STRING) StringId(ANY_STRING)

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

private:
	void LoadModel(const std::string& filePath, bool texturedModel = false);
	void LoadMaterial(const std::string& filePath);
	void LoadTexture(const std::string& filePath);

	void ClearModel(Model* model);
	void ClearMaterial(Material* material);

private:
	ResourceMap m_resources;
	DXRenderer* m_dxrenderer;
};

