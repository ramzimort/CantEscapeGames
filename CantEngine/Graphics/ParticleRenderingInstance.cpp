#include "stdafx.h"
#include "ParticleRenderingInstance.h"
#include "Graphics/AppRenderer.h"


ParticleRenderingInstance::ParticleRenderingInstance(ParticleRendering& particleRendering)
	:m_particleRendering(particleRendering)
{
	m_appRenderer = m_particleRendering.m_appRenderer;
	m_dxrenderer = m_particleRendering.m_dxrenderer;
}


ParticleRenderingInstance::~ParticleRenderingInstance()
{
}

void ParticleRenderingInstance::Render(const AppRendererContext& appRendererContext)
{
	RenderParticles(appRendererContext);
}

void ParticleRenderingInstance::Initialize(const AppRendererContext& appRendererContext)
{

}
void ParticleRenderingInstance::LoadContent(const AppRendererContext& appRendererContext)
{

}
void ParticleRenderingInstance::Release()
{

}

void ParticleRenderingInstance::RenderParticles(const AppRendererContext& appRendererContext)
{
	if (m_particleRendering.m_particleEmitterInstanceList.empty())
	{
		return;
	}

	m_dxrenderer->cmd_bind_render_targets(&appRendererContext.m_appRendererInstance->m_curMainRT, 1,
		appRendererContext.m_appRendererInstance->m_depthRT, nullptr);
	m_dxrenderer->cmd_bind_pipeline(m_particleRendering.m_renderParticlesPipeline);


	for (uint32_t i = 0; i < m_particleRendering.m_particleEmitterInstanceList.size(); ++i)
	{
		auto& particleEmitterInstData = m_particleRendering.m_particleEmitterInstanceList[i];

		m_dxrenderer->cmd_bind_vertex_buffer(particleEmitterInstData.m_pDrawStreamOutVB);

		DescriptorData params[5] = {};
		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[0].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
		params[0].m_buffers = &appRendererContext.m_appRendererInstance->m_camera_uniform_buffer;

		params[1].m_binding_location = 0;
		params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
		params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
		params[1].m_textures = &particleEmitterInstData.m_pParticleTexture;

		params[2].m_binding_location = 0;
		params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
		params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
		params[2].m_samplers = &m_appRenderer->m_texture_sampler;

		m_dxrenderer->cmd_bind_descriptor(m_particleRendering.m_renderParticlesPipeline, 3, params);
		m_dxrenderer->cmd_draw_auto();

	}
}
