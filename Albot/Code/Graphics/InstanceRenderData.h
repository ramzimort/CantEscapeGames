#pragma once
#include "Shaders/Shading.h"

class Light;
class Material;
class Model;
class Texture;
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



struct InstanceRenderData
{
	Matrix model_mat = Matrix::Identity;
	Matrix normal_mat = Matrix::Identity;
	Material* p_ref_material = nullptr;
	Model* p_ref_model = nullptr;
	Vector2 uv_tiling = Vector2(1.0f, 1.0f);
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

typedef std::vector<ParticleEmitterInstanceData> ParticleEmitterInstanceList;
typedef std::vector<InstanceRenderData> InstanceRenderList;