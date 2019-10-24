#include "stdafx.h"
#include "EquirectangularToSkyboxRender.h"
#include "Graphics/AppRenderer.h"


EquirectangularToSkyboxRenderInstance::EquirectangularToSkyboxRenderInstance(EquirectangularToSkyboxRender& parent_render)
	:m_parent_render(parent_render)
{
	m_source_texture_to_skybox_uniform_buffer_list.reserve(16);
	m_source_texture_to_skybox_uniform_data_list.reserve(16);
}

void EquirectangularToSkyboxRenderInstance::Render(Texture* src_texture, Texture* dest_texture)
{
	uint32_t mip_levels = dest_texture->get_desc().m_mipLevels;


	uint32_t skybox_width = dest_texture->get_desc().m_width;
	uint32_t skybox_height = dest_texture->get_desc().m_height;
	
	for (uint32_t mip_index = 0; mip_index < mip_levels; ++mip_index)
	{
		if (mip_index >= m_source_texture_to_skybox_uniform_buffer_list.size())
		{
			AddUniformBuffer();
		}

		m_source_texture_to_skybox_uniform_data_list[mip_index].MipLevel = mip_index;
		m_source_texture_to_skybox_uniform_data_list[mip_index].SkyboxSize = Vector4(float(skybox_width >> mip_index), float(skybox_height >> mip_index), 0.f, 0.f);

		BufferUpdateDesc update_desc = {};
		update_desc.m_buffer = m_source_texture_to_skybox_uniform_buffer_list[mip_index];
		update_desc.m_pSource = &m_source_texture_to_skybox_uniform_data_list[mip_index];
		update_desc.m_size = sizeof(SourceTextureToSkyboxUniformData);

		m_parent_render.m_dxrenderer->cmd_update_buffer(update_desc);
	}

	for (uint32_t mip_index = 0; mip_index < mip_levels; ++mip_index)
	{
		DescriptorData params[4] = {};
		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
		params[0].m_samplers = &m_parent_render.m_appRenderer->m_texture_sampler;
		params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

		params[1].m_binding_location = 0;
		params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
		params[1].m_textures = &src_texture;
		params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

		params[2].m_binding_location = 0;
		params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
		params[2].m_textures = &dest_texture;
		params[2].m_shader_stages = Shader_Stages::COMPUTE_STAGE;
		params[2].m_uav_mip_slice = mip_index;
		
		params[3].m_binding_location = 0;
		params[3].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[3].m_buffers = &m_source_texture_to_skybox_uniform_buffer_list[mip_index];
		params[3].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

		m_parent_render.m_dxrenderer->cmd_bind_descriptor(m_parent_render.m_to_skybox_pipeline, 4, params);

		m_parent_render.m_dxrenderer->cmd_dispatch((uint32_t)m_source_texture_to_skybox_uniform_data_list[mip_index].SkyboxSize.x / TO_SKYBOX_WORKGROUP_THREAD_ID_X,
			(uint32_t)m_source_texture_to_skybox_uniform_data_list[mip_index].SkyboxSize.y / TO_SKYBOX_WORKGROUP_THREAD_ID_Y, 6u);
	}
}

void EquirectangularToSkyboxRenderInstance::Release()
{
	for (Buffer* buffer : m_source_texture_to_skybox_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}
}

void EquirectangularToSkyboxRenderInstance::AddUniformBuffer()
{
	BufferLoadDesc to_skybox_uniform_buffer_desc = {};
	to_skybox_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	to_skybox_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	to_skybox_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	to_skybox_uniform_buffer_desc.m_desc.m_debugName = "To skybox";
	to_skybox_uniform_buffer_desc.m_size = sizeof(SourceTextureToSkyboxUniformData);
	to_skybox_uniform_buffer_desc.m_rawData = nullptr;

	Buffer* source_texture_to_skybox_uniform_buffer =
		DXResourceLoader::Create_Buffer(m_parent_render.m_dxrenderer, to_skybox_uniform_buffer_desc);

	m_source_texture_to_skybox_uniform_buffer_list.push_back(source_texture_to_skybox_uniform_buffer);

	m_source_texture_to_skybox_uniform_data_list.push_back(SourceTextureToSkyboxUniformData{});
}

EquirectangularToSkyboxRender::EquirectangularToSkyboxRender(AppRenderer* app_renderer)
	:m_appRenderer(app_renderer)
{
}


EquirectangularToSkyboxRender::~EquirectangularToSkyboxRender()
{
}

void EquirectangularToSkyboxRender::Release()
{
	SafeReleaseDelete(m_to_skybox_pipeline);
	SafeReleaseDelete(m_to_skybox_shader);

	for (auto& ptr : m_instancesRender)
	{
		SafeReleaseDelete(ptr);
	}

	//SafeReleaseDelete(m_source_texture_to_skybox_uniform_buffer);
}

void EquirectangularToSkyboxRender::Render(Texture* src_texture, Texture* dest_texture)
{
	if (m_curInstancesCount >= m_instancesRender.size())
	{
		m_instancesRender.push_back(new EquirectangularToSkyboxRenderInstance(*this));
	}

	m_instancesRender[m_curInstancesCount]->Render(src_texture, dest_texture);
	++m_curInstancesCount;


	/*m_source_texture_to_skybox_uniform_data.SkyboxSize.x = (float)dest_texture->get_desc().m_width;
	m_source_texture_to_skybox_uniform_data.SkyboxSize.y = (float)dest_texture->get_desc().m_height;

	BufferUpdateDesc update_toskybox_uniform_desc = {};

	update_toskybox_uniform_desc.m_buffer = m_source_texture_to_skybox_uniform_buffer;
	update_toskybox_uniform_desc.m_p_source = &m_source_texture_to_skybox_uniform_data;
	update_toskybox_uniform_desc.m_size = sizeof(SourceTextureToSkyboxUniformData);
	
	m_dxrenderer->cmd_update_buffer(update_toskybox_uniform_desc);

	m_dxrenderer->cmd_bind_pipeline(m_to_skybox_pipeline);
	
	DescriptorData params[4] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
	params[0].m_samplers = &m_appRenderer->m_texture_sampler;
	params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[1].m_binding_location = 0;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[1].m_textures = &src_texture;
	params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[2].m_binding_location = 0;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
	params[2].m_textures = &dest_texture;
	params[2].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[3].m_binding_location = 0;
	params[3].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[3].m_buffers = &m_source_texture_to_skybox_uniform_buffer;
	params[3].m_shader_stages = Shader_Stages::COMPUTE_STAGE;


	m_dxrenderer->cmd_bind_descriptor(m_to_skybox_pipeline, 4, params);
	m_dxrenderer->cmd_dispatch((uint32_t)dest_texture->get_desc().m_width / TO_SKYBOX_WORKGROUP_THREAD_ID_X, 
		(uint32_t)dest_texture->get_desc().m_height / TO_SKYBOX_WORKGROUP_THREAD_ID_Y, 6u);*/

}


void EquirectangularToSkyboxRender::Initialize(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	/*BufferLoadDesc to_skybox_uniform_buffer_desc = {};
	to_skybox_uniform_buffer_desc.m_desc.m_bind_flags = Bind_Flags::BIND_CONSTANT_BUFFER;
	to_skybox_uniform_buffer_desc.m_desc.m_usage_type = Usage_Type::USAGE_DYNAMIC;
	to_skybox_uniform_buffer_desc.m_desc.m_cpu_access_type = CPU_Access_Type::ACCESS_WRITE;
	to_skybox_uniform_buffer_desc.m_desc.m_debug_name = "To skybox";
	to_skybox_uniform_buffer_desc.m_size = sizeof(SourceTextureToSkyboxUniformData);
	to_skybox_uniform_buffer_desc.m_raw_data = nullptr;

	m_source_texture_to_skybox_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, to_skybox_uniform_buffer_desc);*/

	ShaderLoadDesc to_skybox_shader_desc = {};
	to_skybox_shader_desc.m_desc.m_compute_shader_path = "equirectangular_to_skybox_comp.hlsl";

	m_to_skybox_shader = DXResourceLoader::Create_Shader(m_dxrenderer, to_skybox_shader_desc);

	PipelineDesc pipeline_desc = { };
	pipeline_desc.m_pipeline_type = PipelineType::COMPUTE;
	pipeline_desc.m_compute_desc = {};

	ComputePipelineDesc& to_skybox_pipeline_desc = pipeline_desc.m_compute_desc;
	to_skybox_pipeline_desc.m_shader = m_to_skybox_shader;


	m_to_skybox_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}

