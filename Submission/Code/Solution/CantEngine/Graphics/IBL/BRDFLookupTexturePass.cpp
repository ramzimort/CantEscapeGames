#include "stdafx.h"
#include "BRDFLookupTexturePass.h"


BRDFLookupTexturePass::BRDFLookupTexturePass(AppRenderer* appRenderer)
	:m_appRenderer(appRenderer)
{
}


BRDFLookupTexturePass::~BRDFLookupTexturePass()
{
}


void BRDFLookupTexturePass::Initialize(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	ShaderLoadDesc brdf_lookup_texture_pass_shader_desc = {};
	brdf_lookup_texture_pass_shader_desc.m_desc.m_compute_shader_path = "process_brdf_lut_comp.hlsl";

	m_brdf_lookup_texture_pass_shader = DXResourceLoader::Create_Shader(m_dxrenderer, brdf_lookup_texture_pass_shader_desc);

	PipelineDesc pipeline_desc = { };
	pipeline_desc.m_pipeline_type = PipelineType::COMPUTE;
	pipeline_desc.m_compute_desc = {};

	ComputePipelineDesc& brdf_lookup_texture_pipeline = pipeline_desc.m_compute_desc;
	brdf_lookup_texture_pipeline.m_shader = m_brdf_lookup_texture_pass_shader;

	m_brdf_lookup_texture_pass_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	BufferLoadDesc process_brdf_lookup_uniform_buffer_desc = {};
	process_brdf_lookup_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	process_brdf_lookup_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	process_brdf_lookup_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	process_brdf_lookup_uniform_buffer_desc.m_size = sizeof(ProcessBRDFLookupTextureUniformData);
	process_brdf_lookup_uniform_buffer_desc.m_rawData = nullptr;

	m_process_brdf_lookup_texture_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, process_brdf_lookup_uniform_buffer_desc);
}

void BRDFLookupTexturePass::Release()
{
	SafeReleaseDelete(m_brdf_lookup_texture_pass_shader);
	SafeReleaseDelete(m_brdf_lookup_texture_pass_pipeline);
	SafeReleaseDelete(m_process_brdf_lookup_texture_uniform_buffer);
}

void BRDFLookupTexturePass::Render(Texture* brdf_lut_texture)
{
	m_process_brdf_lookup_texture_uniform_data.BRDFLUTTextureSize.x = (float)brdf_lut_texture->get_desc().m_width;
	m_process_brdf_lookup_texture_uniform_data.BRDFLUTTextureSize.y = (float)brdf_lut_texture->get_desc().m_height;

	BufferUpdateDesc update_brdf_lookup_texture_desc = {};
	update_brdf_lookup_texture_desc.m_buffer = m_process_brdf_lookup_texture_uniform_buffer;
	update_brdf_lookup_texture_desc.m_pSource = &m_process_brdf_lookup_texture_uniform_data;
	update_brdf_lookup_texture_desc.m_size = sizeof(ProcessBRDFLookupTextureUniformData);
	m_dxrenderer->cmd_update_buffer(update_brdf_lookup_texture_desc);


	m_dxrenderer->cmd_bind_pipeline(m_brdf_lookup_texture_pass_pipeline);

	DescriptorData params[2] = {};
	params[0].m_binding_location = 0;
	params[0].m_buffers = &m_process_brdf_lookup_texture_uniform_buffer;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[1].m_binding_location = 0;
	params[1].m_textures = &brdf_lut_texture;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
	params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	m_dxrenderer->cmd_bind_descriptor(m_brdf_lookup_texture_pass_pipeline, 2, params);
	
	m_dxrenderer->cmd_dispatch(brdf_lut_texture->get_desc().m_width / PROCESS_BRDF_LUT_WORKGROUP_THREAD_ID_X, 
		brdf_lut_texture->get_desc().m_height / PROCESS_BRDF_LUT_WORKGROUP_THREAD_ID_Y, 1u);
}