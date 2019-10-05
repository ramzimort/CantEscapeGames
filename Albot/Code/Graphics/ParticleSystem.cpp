#include "ParticleSystem.h"
#include "Graphics/AppRenderer.h"


ParticleSystem::ParticleSystem(AppRenderer* appRenderer)
	:m_appRenderer(appRenderer),
	m_firstTime(true),
	m_maxParticlesCount(10000)
{
}


ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::LoadContent(DXRenderer* dxrenderer)
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

	ShaderLoadDesc streamoutParticleShaderDesc = {};
	streamoutParticleShaderDesc.m_desc.m_vertex_shader_path = "streamout_particle_vert.hlsl";
	streamoutParticleShaderDesc.m_desc.m_geometry_shader_path = "streamout_particle_geom.hlsl";

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

	Particle initParticle = {};
	initParticle.m_lifeTime = 0.f;
	initParticle.m_position = Vector3(0.f);
	initParticle.m_size = Vector2(1.f, 1.f);
	initParticle.m_particleType = PARTICLE_TYPE_EMITTER;


	BufferLoadDesc streamout_vb_desc = {};
	streamout_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
	streamout_vb_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	streamout_vb_desc.m_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	streamout_vb_desc.m_desc.m_vertexStride = sizeof(Particle);
	streamout_vb_desc.m_rawData = &initParticle;
	streamout_vb_desc.m_size = streamout_vb_desc.m_desc.m_vertexStride * 1;

	m_initVB = DXResourceLoader::Create_Buffer(m_dxrenderer, streamout_vb_desc);
	
	streamout_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER | Bind_Flags::BIND_STREAM_OUTPUT;
	streamout_vb_desc.m_size = streamout_vb_desc.m_desc.m_vertexStride * m_maxParticlesCount;
	streamout_vb_desc.m_rawData = nullptr;

	m_streamOutVB = DXResourceLoader::Create_Buffer(m_dxrenderer, streamout_vb_desc);
	m_drawStreamOutVB = DXResourceLoader::Create_Buffer(m_dxrenderer, streamout_vb_desc);


	BufferLoadDesc streamout_uniform_vb_desc = {};
	streamout_uniform_vb_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	streamout_uniform_vb_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	streamout_uniform_vb_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	streamout_uniform_vb_desc.m_desc.m_debugName = "Stream out particle uniform";
	streamout_uniform_vb_desc.m_size = sizeof(ParticleStreamOutUniformData);
	streamout_uniform_vb_desc.m_rawData = nullptr;

	m_particleStreamOutUniformBuffer = DXResourceLoader::Create_Buffer(m_dxrenderer, streamout_uniform_vb_desc);
}


void ParticleSystem::Render()
{
	m_dxrenderer->cmd_bind_pipeline(m_geomParticleLifetimePipeline);
	m_dxrenderer->cmd_bind_render_targets(nullptr, 0, nullptr, nullptr);
	if (m_firstTime)
	{
		m_dxrenderer->cmd_bind_vertex_buffer(m_initVB);
	}
	else
	{
		m_dxrenderer->cmd_bind_vertex_buffer(m_drawStreamOutVB);
	}

	m_dxrenderer->cmd_bind_streamout_render_targets(m_streamOutVB, 0);

	BufferUpdateDesc updateParticleStreamoutUniformBuffer = {};
	updateParticleStreamoutUniformBuffer.m_buffer = m_particleStreamOutUniformBuffer;
	updateParticleStreamoutUniformBuffer.m_pSource = &m_particleStreamOutUniformData;
	updateParticleStreamoutUniformBuffer.m_size = sizeof(ParticleStreamOutUniformData);
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
	streamoutParams[2].m_buffers = &m_particleStreamOutUniformBuffer;
	streamoutParams[2].m_shader_stages = Shader_Stages::GEOMETRY_STAGE;
	streamoutParams[2].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;

	m_dxrenderer->cmd_bind_descriptor(m_geomParticleLifetimePipeline, 3, streamoutParams);

	if (m_firstTime)
	{
		m_dxrenderer->cmd_draw(1, 0);
		m_firstTime = false;
	}
	else
	{
		m_dxrenderer->cmd_draw_auto();
	}
	m_dxrenderer->cmd_bind_streamout_render_targets(nullptr, 0);
	std::swap(m_drawStreamOutVB, m_streamOutVB);
}


void ParticleSystem::Release()
{
	SafeReleaseDelete(m_particleStreamOutUniformBuffer);
	SafeReleaseDelete(m_initVB);
	SafeReleaseDelete(m_drawStreamOutVB);
	SafeReleaseDelete(m_streamOutVB);
	SafeReleaseDelete(m_geomParticleLifetimePipeline);
	SafeReleaseDelete(m_geomParticleLifetimeShader);
	SafeReleaseDelete(m_renderParticlesPipeline);
	SafeReleaseDelete(m_renderParticlesShader);
}

void ParticleSystem::Update(float dt, float gameTime)
{
	m_particleStreamOutUniformData.DeltaTime = dt;
	m_particleStreamOutUniformData.GameTime = gameTime;
	m_particleStreamOutUniformData.EmitterDirection = Vector4(0.f, 1.0f, 0.0f, 1.f);
	m_particleStreamOutUniformData.EmitterPosition = Vector4(0.f, 20.f, -20.f, 1.f);
}