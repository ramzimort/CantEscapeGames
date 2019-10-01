#pragma once

#include "Helper/Hash.h"

class Texture;

class Material
{
public:
	friend class ResourceManager;
	friend class ModelLoader;
	friend class Factory;
public:
	Material();
	~Material();

	void Init();

	//Check Shaders/Shading.h
	uint32_t GetShaderMaterialType() const;
	Texture* GetDiffuseTexture() const;
	Texture* GetNormalTexture()const ;
	Texture* GetHeightTexture()const;
	Vector4 GetDiffuseColor() const;
	Vector4 GetSpecularColor() const;

private:
	Vector4 m_diffuseColor;
	Vector4 m_specularColor;
	Texture* m_pDiffuseTexture;
	Texture* m_pNormalTexture;
	Texture* m_pHeightTexture;
	StringId m_materialId;
	StringId m_diffuseTextureId;
	StringId m_normalTextureId;
	StringId m_heightTextureId;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

