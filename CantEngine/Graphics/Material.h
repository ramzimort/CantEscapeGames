#pragma once

#include "Helper/Hash.h"

/**
 * @file Material.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public interface for describing how a 3D mesh should be rendered
 */


class Texture;

/**
 * @brief Material describes how a 3D mesh should be rendered
 * 
 */
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
	/**
	 * @brief Get the Shader Material Type ID
	 * 
	 * @return uint32_t 
	 */
	uint32_t GetShaderMaterialType() const;
	/**
	 * @brief Get the Diffuse Texture pointer
	 * 
	 * @return Texture* 
	 */
	Texture* GetDiffuseTexture() const;
	/**
	 * @brief Get the Normal Texture pointer. This gives Normal Mapping effect
	 * 
	 * @return Texture* 
	 */
	Texture* GetNormalTexture()const;
	/**
	 * @brief Get the Height Texture pointer. This gives Parallax Mapping effect
	 * 
	 * @return Texture* 
	 */
	Texture* GetHeightTexture()const;
	/**
	 * @brief Get the Specular Texture pointer
	 * 
	 * @return Texture* 
	 */
	Texture* GetSpecularTexture() const;
	/**
	 * @brief Get the Metallic Texture pointer. This define the PBR metallic map
	 * 
	 * @return Texture* 
	 */
	Texture* GetMetallicTexture() const;
	/**
	 * @brief Get the Roughness Texture pointer. This define the PBR roughness map
	 * 
	 * @return Texture* 
	 */
	Texture* GetRoughnessTexture() const;
	/**
	 * @brief Get the Diffuse Color of the 3D mesh. Only valid if there is no Diffuse Texture
	 * 
	 * @return Vector4 
	 */
	Vector4 GetDiffuseColor() const;
	/**
	 * @brief Get the Specular Color of the 3D mesh. Only valid if there is no Specular Texture
	 * 
	 * @return Vector4 
	 */
	Vector4 GetSpecularColor() const;
	/**
	 * @brief Get the Metallic Value of the 3D mesh. Only valid if there is no Metallic Texture
	 * 
	 * @return float 
	 */
	float GetMetallicValue() const;
	/**
	 * @brief Get the Roughness Value of the 3D mesh. Only valid if there is no Roughness Texture
	 * 
	 * @return float 
	 */
	float GetRoughnessValue() const;

private:
	Vector4 m_diffuseColor;
	Vector4 m_specularColor;
	Texture* m_pDiffuseTexture;
	Texture* m_pNormalTexture;
	Texture* m_pHeightTexture;
	Texture* m_pSpecularTexture;
	Texture* m_pMetallicTexture;
	Texture* m_pRoughnessTexture;
	StringId m_materialId;
	StringId m_diffuseTextureId;
	StringId m_normalTextureId;
	StringId m_heightTextureId;
	StringId m_specularTextureId;
	StringId m_metallicTextureId;
	StringId m_roughnessTextureId;
	float m_metallicValue;
	float m_roughnessValue;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

