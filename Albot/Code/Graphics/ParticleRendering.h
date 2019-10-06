#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/InstanceRenderData.h"

struct Particle
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector2 m_size;
	float m_lifeTime;
	uint32_t m_particleType;
};

class AppRenderer;

class ParticleRendering
{
public:
	friend class AppRenderer;
public:
	ParticleRendering(AppRenderer* appRenderer);
	~ParticleRendering();

	void LoadContent(DXRenderer* dxrenderer);
	void Render();
	void Release();
	void Update(float dt, float gameTime);


	void RegisterParticleEmitterInstance(const ParticleEmitterInstanceData& particleEmmiterInstanceData);
private:
	void RenderStreamoutProcess();
	void RenderParticles();
private:
	ParticleEmitterInstanceList m_particleEmitterInstanceList;

	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Shader* m_geomParticleLifetimeShader;
	Pipeline* m_geomParticleLifetimePipeline;

	Pipeline* m_renderParticlesPipeline;
	Shader* m_renderParticlesShader;


	Buffer* m_particleStreamOutUniformBuffer;
	ParticleEmitterStreamOutUniformData m_particleStreamOutUniformData;

	Buffer* m_initVB;
	Buffer* m_streamOutVB;
	Buffer* m_drawStreamOutVB;

	uint64_t m_maxParticlesCount;

	bool m_firstTime;


	//TODO: need to be removed, only for testing purpose
	Texture* m_flareTexture;

	
};

