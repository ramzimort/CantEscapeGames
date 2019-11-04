#pragma once
#include "Shaders/Shading.h"


typedef std::vector<Matrix> BoneTransformationsList;

class Light;
class Material;
class Model;
class Texture;
class HaloEffect;
struct ParticleEmitterUniformData;
class Buffer;

struct PointLightInstanceData
{
	const Light* light;
	Vector3 light_position;
};

struct DirectionalLightInstanceData
{
	const Light* light;
	Vector3 light_direction;
};


struct HaloEffectInstanceData
{
	const HaloEffect* haloEffect;
	Vector3 m_haloPosition;
};


struct InstanceRenderData
{
	Matrix model_mat = Matrix::Identity;
	Matrix normal_mat = Matrix::Identity;
	Material* p_ref_material = nullptr;
	Model* p_ref_model = nullptr;
	Vector2 uv_tiling = Vector2(1.0f, 1.0f);
};


struct BoneMeshInstanceRenderData
{
	InstanceRenderData m_instanceRenderData;
	BoneTransformationsList* m_pBoneTransformationsList;
};


struct ParticleEmitterInstanceData
{
	ParticleEmitterUniformData m_pParticleEmitterUniformData;
	Buffer* m_pInitVB;
	Buffer* m_pStreamOutVB;
	Buffer* m_pDrawStreamOutVB;
	Texture* m_pParticleTexture;
	bool m_firstTime;
};

struct BakedSkyboxIrradianceInstanceData
{
	Texture* m_pSkyboxTexture;
	Texture* m_pSkyboxIrradianceTexture;
	Texture* m_pIBLPrefilteredEnvMapTexture;
};

struct ProcessSkyboxIrradianceInstanceData
{
	Texture* m_pSkyboxEquirectangularTexture;
	Texture* m_pSkyboxEquirectangularIrradianceTexture;
	bool* m_hasBaked;
	BakedSkyboxIrradianceInstanceData m_bakedData;
};

struct UIObjectInstanceRenderData
{
	//window space is 0-window_width & 0-window_height
	//origin in top left of the UI object
	Vector3 m_windowSpacePosition;
	Vector3 m_windowSpaceSize;
	//you fill this pointer with the address of m_rotationMat from m_rotMatrix
	const Matrix* m_rotationMatrix;
	Material* m_pUIMaterial;
};


typedef std::vector<ParticleEmitterInstanceData> ParticleEmitterInstanceList;
typedef std::vector<InstanceRenderData> InstanceRenderList;
typedef std::vector<HaloEffectInstanceData> HaloEffectInstanceList;
typedef std::vector<PointLightInstanceData > PointLightInstanceDataList;
typedef std::vector<DirectionalLightInstanceData> DirectionalLightInstanceDataList;
typedef std::vector<BoneMeshInstanceRenderData> BoneMeshInstancesRenderDataList;
typedef std::vector< ProcessSkyboxIrradianceInstanceData> ProcessSkyboxIrradianceInstanceDataList;
typedef std::vector< BakedSkyboxIrradianceInstanceData> BakedSkyboxIrradianceInstanceDataList;
typedef std::vector<UIObjectInstanceRenderData> UIObjectInstanceRenderDataList;
