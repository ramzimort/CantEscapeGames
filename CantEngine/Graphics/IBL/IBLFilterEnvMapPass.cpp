#include "stdafx.h"
#include "IBLFilterEnvMapPass.h"
#include "Graphics/AppRenderer.h"

IBLFilterEnvMapPass::IBLFilterEnvMapPass(AppRenderer* appRenderer)
	:m_app_renderer(appRenderer)
{
	m_ibl_filter_env_map_uniform_buffer_list.reserve(16);
	m_ibl_filter_env_map_uniform_data_list.reserve(16);
}


IBLFilterEnvMapPass::~IBLFilterEnvMapPass()
{
}


void IBLFilterEnvMapPass::Release()
{
	SafeReleaseDelete(m_ibl_filter_env_map_pipeline);
	SafeReleaseDelete(m_ibl_filter_env_map_shader);

	for (Buffer* buffer : m_ibl_filter_env_map_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}
}

void IBLFilterEnvMapPass::initialize(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	ShaderLoadDesc ibl_filter_shader_desc = {};
	ibl_filter_shader_desc.m_desc.m_compute_shader_path = "ibl_prefilter_env_map_comp.hlsl";

	m_ibl_filter_env_map_shader = DXResourceLoader::Create_Shader(m_dxrenderer, ibl_filter_shader_desc);

	PipelineDesc pipeline_desc = { };
	pipeline_desc.m_pipeline_type = PipelineType::COMPUTE;
	pipeline_desc.m_compute_desc = {};

	ComputePipelineDesc& ibl_filter_pipeline_desc = pipeline_desc.m_compute_desc;
	ibl_filter_pipeline_desc.m_shader = m_ibl_filter_env_map_shader;

	m_ibl_filter_env_map_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}

void IBLFilterEnvMapPass::render(Texture* src_skybox_texture, Texture* dest_filtered_env_map)
{
	uint32_t mip_levels = dest_filtered_env_map->get_desc().m_mip_levels;
	uint32_t skybox_width = dest_filtered_env_map->get_desc().m_width;
	uint32_t skybox_height = dest_filtered_env_map->get_desc().m_height;

	for (uint32_t mip_index = 0; mip_index < mip_levels; ++mip_index)
	{
		if (mip_index >= m_ibl_filter_env_map_uniform_buffer_list.size())
		{
			add_uniform_buffer();
		}

		m_ibl_filter_env_map_uniform_data_list[mip_index].SourceSkyboxSize.x = (float)src_skybox_texture->get_desc().m_width;
		m_ibl_filter_env_map_uniform_data_list[mip_index].SourceSkyboxSize.y = (float)src_skybox_texture->get_desc().m_height;
		m_ibl_filter_env_map_uniform_data_list[mip_index].MipLevel = mip_index;
		m_ibl_filter_env_map_uniform_data_list[mip_index].SkyboxSize = Vector4((skybox_width >> mip_index), (skybox_height >> mip_index), 0.f, 0.f);
		m_ibl_filter_env_map_uniform_data_list[mip_index].MiscData.x = (float)(mip_index) / (float)(mip_levels - 1);


		BufferUpdateDesc update_desc = {};
		update_desc.m_buffer = m_ibl_filter_env_map_uniform_buffer_list[mip_index];
		update_desc.m_p_source = &m_ibl_filter_env_map_uniform_data_list[mip_index];
		update_desc.m_size = sizeof(IBLFilterEnvMapUniformData);

		m_dxrenderer->cmd_update_buffer(update_desc);
	}

	m_dxrenderer->cmd_bind_pipeline(m_ibl_filter_env_map_pipeline);

	DescriptorData params[2] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
	params[0].m_samplers = &m_app_renderer->m_texture_sampler;
	params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[1].m_binding_location = 0;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[1].m_textures = &src_skybox_texture;
	params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	

	m_dxrenderer->cmd_bind_descriptor(m_ibl_filter_env_map_pipeline, 2, params);

	for (uint32_t mip_index = 0; mip_index < mip_levels; ++mip_index)
	{
		DescriptorData unique_params[2] = {};

		unique_params[0].m_binding_location = 0;
		unique_params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
		unique_params[0].m_textures = &dest_filtered_env_map;
		unique_params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;
		unique_params[0].m_uav_mip_slice = mip_index;

		unique_params[1].m_binding_location = 0;
		unique_params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		unique_params[1].m_buffers = &m_ibl_filter_env_map_uniform_buffer_list[mip_index];
		unique_params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

		m_dxrenderer->cmd_bind_descriptor(m_ibl_filter_env_map_pipeline, 2, unique_params);

		m_dxrenderer->cmd_dispatch((uint32_t)m_ibl_filter_env_map_uniform_data_list[mip_index].SkyboxSize.x / IBL_ENV_MAP_FILTER_WORKGROUP_THREAD_ID_X,
			(uint32_t)m_ibl_filter_env_map_uniform_data_list[mip_index].SkyboxSize.y / IBL_ENV_MAP_FILTER_WORKGROUP_THREAD_ID_Y, 6u);
	}
}


void IBLFilterEnvMapPass::add_uniform_buffer()
{
	BufferLoadDesc ibl_filter_uniform_buffer_desc = {};
	ibl_filter_uniform_buffer_desc.m_desc.m_bind_flags = Bind_Flags::BIND_CONSTANT_BUFFER;
	ibl_filter_uniform_buffer_desc.m_desc.m_usage_type = Usage_Type::USAGE_DYNAMIC;
	ibl_filter_uniform_buffer_desc.m_desc.m_cpu_access_type = CPU_Access_Type::ACCESS_WRITE;
	ibl_filter_uniform_buffer_desc.m_desc.m_debug_name = "To skybox";
	ibl_filter_uniform_buffer_desc.m_size = sizeof(IBLFilterEnvMapUniformData);
	ibl_filter_uniform_buffer_desc.m_raw_data = nullptr;

	Buffer* buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, ibl_filter_uniform_buffer_desc);

	m_ibl_filter_env_map_uniform_buffer_list.push_back(buffer);

	m_ibl_filter_env_map_uniform_data_list.push_back(IBLFilterEnvMapUniformData{});
}
