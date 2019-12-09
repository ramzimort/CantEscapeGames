#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/InstanceRenderData.h"

/**
 * @file ParticleRendering.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public high level interface particle rendering available for users to use
 */


struct Particle
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector2 m_size;
	float m_lifeTime;
	uint32_t m_particleType;
};

class AppRenderer;

/**
 * @brief Contains all the interface for rendering particle
 * 
 */
class ParticleRendering
{
public:
	friend class AppRenderer;
	friend class ParticleRenderingInstance;
public:
	ParticleRendering(AppRenderer* appRenderer);
	~ParticleRendering();

	/**
	 * @brief Load all of the high level resources for particle rendering
	 * 
	 * @param dxrenderer 
	 */
	void LoadContent(DXRenderer* dxrenderer);
	/**
	 * @brief Render the render info list related to particle rendering
	 * 
	 */
	void Render();
	/**
	 * @brief Release all the graphic resources related for Particle Rendering
	 * 
	 */
	void Release();
	void Update(float dt, float gameTime);

	/**
	 * @brief Pass render info request for particle rendering
	 * 
	 * @param particleEmmiterInstanceData 
	 */
	void RegisterParticleEmitterInstance(const ParticleEmitterInstanceData& particleEmmiterInstanceData);

	/**
	 * @brief Clear all of the render info request related to particle rendering
	 * 
	 */
	void ClearInstances();
private:
	void RenderStreamoutProcess();
	void RenderParticles();
	void AddParticleStreamOutUniformBuffer();
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Shader* m_geomParticleLifetimeShader;
	Pipeline* m_geomParticleLifetimePipeline;

	Pipeline* m_renderParticlesPipeline;
	Shader* m_renderParticlesShader;

	std::deque<ParticleEmitterStreamOutUniformData> m_particleEmitterStreamOutUniformList;
	ParticleEmitterInstanceList m_particleEmitterInstanceList;
	std::deque<Buffer*> m_particleEmitterStreamOutUniformBufferList;
};

