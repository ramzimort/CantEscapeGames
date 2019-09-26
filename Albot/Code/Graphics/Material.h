#pragma once
#include <string>


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
	std::string m_materialName;
	std::string m_diffuseTextureDirectory;
	std::string m_normalTextureDirectory;
	std::string m_heightTextureDirectory;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

