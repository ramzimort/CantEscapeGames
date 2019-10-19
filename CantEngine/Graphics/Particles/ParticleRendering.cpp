#include "ParticleRendering.h"
#include "Graphics/AppRenderer.h"
#include "Managers/ResourceManager.h"


ParticleRendering::ParticleRendering(AppRenderer* appRenderer)
	:m_appRenderer(appRenderer)
{
}


ParticleRendering::~ParticleRendering()
{
}


void ParticleRendering::LoadContent(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	VertexLayout particleLifetimeVertexLayout = {};
	particleLifetimeVertexLayout.m_atrrib_count = 5;
	particleLifetimeVertexLayout.m_attribs[0].m_binding = 0;
	particleLifetimeVertexLayout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	particleLifetimeVertexLayout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;

	particleLifetimeVertexLayout.m_attribs[1].m_binding = 0;
	particleLifetimeVertexLayout.m_attribs[1].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	particleLifetimeVertexLayout.m_attribs[1].m_semantic = Attrib_Semantic::TEXCOORD_0;

	particleLifetimeVertexLayout.m_attribs[2].m_binding = 0;
	particleLifetimeVertexLayout.m_attribs[2].m_format = DXGI_FORMAT_R32G32_FLOAT;
	particleLifetimeVertexLayout.m_attribs[2].m_semantic = Attrib_Semantic::TEXCOORD_1;

	particleLifetimeVertexLayout.m_attribs[3].m_binding = 0;
	particleLifetimeVertexLayout.m_attribs[3].m_format = DXGI_FORMAT_R32_FLOAT;
	particleLifetimeVertexLayout.m_attribs[3].m_semantic = Attrib_Semantic::TEXCOORD_2;


	particleLifetimeVertexLayout.m_attribs[4].m_binding = 0;
	particleLifetimeVertexLayout.m_attribs[4].m_format = DXGI_FORMAT_R32_UINT;
	particleLifetimeVertexLayout.m_attribs[4].m_semantic = Attrib_Semantic::TEXCOORD_3;


	GeometryShaderStreamoutDesc geomShaderStreamoutDesc = {};
	geomShaderStreamoutDesc.m_vertexLayout = &particleLifetimeVertexLayout;

	ShaderLoadDesc streamoutParticleShaderDesc = {};
	streamoutParticleShaderDesc.m_desc.m_vertex_shader_path = "streamout_particle_vert.hlsl";
	streamoutParticleShaderDesc.m_desc.m_geometry_shader_path = "streamout_particle_geom.hlsl";
	streamoutParticleShaderDesc.m_pGeomShaderStreamoutDesc = &geomShaderStreamoutDesc;

	m_geomParticleLifetimeShader = DXResourceLoader::Create_Shader(m_dxrenderer, streamoutParticleShaderDesc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;
	pipeline_desc.m_graphics_desc = {};


	GraphicsPipelineDesc& particleLifetimePipeline = pipeline_desc.m_graphics_desc;
	particleLifetimePipeline.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_POINT_LIST;
	particleLifetimePipeline.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_state;
	particleLifetimePipeline.m_shader = m_geomParticleLifetimeShader;
	particleLifetimePipeline.m_depth_state = m_appRenderer->m_disabled_depth_state;
	particleLifetimePipeline.m_render_target_count = 0;
	particleLifetimePipeline.m_vertex_layout = &particleLifetimeVertexLayout;

	m_geomParticleLifetimePipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	ShaderLoadDesc renderParticleShaderDesc = {};
	renderParticleShaderDesc.m_desc.m_vertex_shader_path = "fire_streamout_particle_vert.hlsl";
	renderParticleShaderDesc.m_desc.m_geometry_shader_path = "fire_streamout_particle_geom.hlsl";
	renderParticleShaderDesc.m_desc.m_pixel_shader_path = "fire_streamout_particle_frag.hlsl";

	m_renderParticlesShader = DXResourceLoader::Create_Shader(m_dxrenderer, renderParticleShaderDesc);

	pipeline_desc.m_graphics_desc = {};
	GraphicsPipelineDesc& renderParticlePipeline = pipeline_desc.m_graphics_desc;
	renderParticlePipeline.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_POINT_LIST;
	renderParticlePipeline.m_render_target_count = 1;
	renderParticlePipeline.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_ms_state;
	renderParticlePipeline.m_depth_state = m_appRenderer->m_testonlyLessEqualDepthState;
	renderParticlePipeline.m_shader = m_renderParticlesShader;
	renderParticlePipeline.m_vertex_layout = &particleLifetimeVertexLayout;
	renderParticlePipeline.m_blend_state = m_appRenderer->m_additiveBlending;
	m_renderParticlesPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}


void ParticleRendering::Render()
{
	RenderStreamoutProcess();
}

void ParticleRendering::ClearInstances()
{
	m_particleEmitterInstanceList.clear();
	m_particleEmitterStreamOutUniformList.clear();
}

void ParticleRendering::RenderStreamoutProcess()
{
	if (m_particleEmitterInstanceList.empty())
	{
		return;
	}

	m_dxrenderer->cmd_bind_pipeline(m_geomParticleLifetimePipeline);
	m_dxrenderer->cmd_bind_render_targets(nullptr, 0, nullptr, nullptr);

	for (uint32_t i = 0; i < m_particleEmitterInstanceList.size(); ++i)
	{
		if (i >= m_particleEmitterStreamOutUniformBufferList.size())
		{
			AddParticleStreamOutUniformBuffer();
		}
		Buffer* curParticleStreamoutUniformBufferData = m_particleEmitterStreamOutUniformBufferList[i];

		auto& particleEmitterInstData = m_particleEmitterInstanceList[i];
		auto& particleEmitterStreamoutInstData = m_particleEmitterStreamOutUniformList[i];

		if (particleEmitterInstData.m_firstTime)
		{
			m_dxrenderer->cmd_bind_vertex_buffer(particleEmitterInstData.m_pInitVB);
		}
		else
		{
			m_dxrenderer->cmd_bind_vertex_buffer(particleEmitterInstData.m_pDrawStreamOutVB);
		}

		m_dxrenderer->cmd_bind_streamout_render_targets(particleEmitterInstData.m_pStreamOutVB, 0);

		BufferUpdateDesc updateParticleStreamoutUniformBuffer = {};
		updateParticleStreamoutUniformBuffer.m_buffer = curParticleStreamoutUniformBufferData;
		updateParticleStreamoutUniformBuffer.m_pSource = &particleEmitterStreamoutInstData;
		updateParticleStreamoutUniformBuffer.m_size = sizeof(ParticleEmitterStreamOutUniformData);
		m_dxrenderer->cmd_update_buffer(updateParticleStreamoutUniformBuffer);


		DescriptorData streamoutParams[3] = {};
		streamoutParams[0].m_binding_location = 0;
		streamoutParams[0].m_textures = &m_appRenderer->m_random1DTexture;
		streamoutParams[0].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
		streamoutParams[0].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;

		streamoutParams[1].m_binding_location = 0;
		streamoutParams[1].m_samplers = &m_appRenderer->m_repeat_linear_sampler;
		streamoutParams[1].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
		streamoutParams[1].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;

		streamoutParams[2].m_binding_location = 0;
		streamoutParams[2].m_buffers = &curParticleStreamoutUniformBufferData;
		streamoutParams[2].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
		streamoutParams[2].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;

		m_dxrenderer->cmd_bind_descriptor(m_geomParticleLifetimePipeline, 3, streamoutParams);

		if (particleEmitterInstData.m_firstTime)
		{
			m_dxrenderer->cmd_draw(1, 0);
		}
		else
		{
			m_dxrenderer->cmd_draw_auto();
		}
		m_dxrenderer->cmd_bind_streamout_render_targets(nullptr, 0);
	}
}

void ParticleRendering::AddParticleStreamOutUniformBuffer()
{
	BufferLoadDesc streamout_uniform_vb_desc = {};
	streamout_uniform_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	streamout_uniform_vb_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	streamout_uniform_vb_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	streamout_uniform_vb_desc.m_desc.m_debugName = "Stream out particle uniform";
	streamout_uniform_vb_desc.m_size = sizeof(ParticleEmitterStreamOutUniformData);
	streamout_uniform_vb_desc.m_rawData = nullptr;

	Buffer* uniformBuffer = DXResourceLoader::Create_Buffer(m_dxrenderer, streamout_uniform_vb_desc);

	m_particleEmitterStreamOutUniformBufferList.push_back(uniformBuffer);
}

void ParticleRendering::RenderParticles()
{
	/*if (m_particleEmitterInstanceList.empty())
	{
		return;
	}

	m_dxrenderer->cmd_bind_render_targets(&m_appRenderer->m_cur_main_rt, 1,
		m_appRenderer->m_depth_rt, nullptr);
	m_dxrenderer->cmd_bind_pipeline(m_renderParticlesPipeline);
	

	for (uint32_t i = 0; i < m_particleEmitterInstanceList.size(); ++i)
	{
		auto& particleEmitterInstData = m_particleEmitterInstanceList[i];

		m_dxrenderer->cmd_bind_vertex_buffer(particleEmitterInstData.m_pDrawStreamOutVB);

		DescriptorData params[5] = {};
		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[0].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
		params[0].m_buffers = &m_appRenderer->m_camera_uniform_buffer;

		params[1].m_binding_location = 0;
		params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
		params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
		params[1].m_textures = &particleEmitterInstData.m_pParticleTexture;

		params[2].m_binding_location = 0;
		params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
		params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
		params[2].m_samplers = &m_appRenderer->m_texture_sampler;

		m_dxrenderer->cmd_bind_descriptor(m_renderParticlesPipeline, 3, params);
		m_dxrenderer->cmd_draw_auto();

	}*/
}

void ParticleRendering::Release()
{
	SafeReleaseDelete(m_geomParticleLifetimePipeline);
	SafeReleaseDelete(m_geomParticleLifetimeShader);
	SafeReleaseDelete(m_renderParticlesPipeline);
	SafeReleaseDelete(m_renderParticlesShader);

	for (Buffer* buffer : m_particleEmitterStreamOutUniformBufferList)
	{
		SafeReleaseDelete(buffer);
	}
}

void ParticleRendering::Update(float dt, float gameTime)
{
	for (uint32_t i = 0; i < m_particleEmitterInstanceList.size(); ++i)
	{
		ParticleEmitterStreamOutUniformData streamOutUniformData = {};
		streamOutUniformData.EmitterData = m_particleEmitterInstanceList[i].m_pParticleEmitterUniformData;
		streamOutUniformData.DeltaTime = dt;
		streamOutUniformData.GameTime = gameTime;
		m_particleEmitterStreamOutUniformList.push_back(streamOutUniformData);
	}
}


void ParticleRendering::RegisterParticleEmitterInstance(
	const ParticleEmitterInstanceData& particleEmmiterInstanceData)
{
	m_particleEmitterInstanceList.push_back(particleEmmiterInstanceData);
}