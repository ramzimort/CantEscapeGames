#include "stdafx.h"
#include "BlurImagePass.h"


BlurImagePass::BlurImagePass(AppRenderer* app_renderer, BlurChannel blur_channel, uint32_t blur_width)
	:m_appRenderer(app_renderer),
	m_blur_width(blur_width),
	m_blur_channel_type(blur_channel)
{
}


BlurImagePass::~BlurImagePass()
{
}

void BlurImagePass::SetBlurWidth(uint32_t blur_width)
{
	m_blur_width = blur_width;
}

void BlurImagePass::Release()
{
	SafeReleaseDelete(m_blur_shader);
	SafeReleaseDelete(m_blur_pipeline);
	SafeReleaseDelete(m_horizontal_blur_uniform_buffer);
	SafeReleaseDelete(m_vertical_blur_uniform_buffer);
}

void BlurImagePass::BlurImage(Texture* input_texture, Texture* intermediate_texture, Texture* output_texture_to_blur)
{
	m_dxrenderer->cmd_bind_pipeline(m_blur_pipeline);

	
	m_vertical_blur_uniform_data.BlurWidth = m_horizontal_blur_uniform_data.BlurWidth = (float)m_blur_width;
	m_vertical_blur_uniform_data.BlurWidth2 = m_horizontal_blur_uniform_data.BlurWidth2 =  (float)m_blur_width * 2;


	uint32_t s_val = m_blur_width / 2;

	float weight_result = 0;
	uint32_t cur_index = 0;
	for (int32_t i = (int32_t)(-m_horizontal_blur_uniform_data.BlurWidth); i <= (int32_t)(m_horizontal_blur_uniform_data.BlurWidth); ++i)
	{
		float i_over_s = (static_cast<float>(i) / static_cast<float>(s_val));
		float i_over_s_power2 = std::pow(i_over_s, 2.f);
		float exponent_value = -0.5f * i_over_s_power2;
		m_vertical_blur_uniform_data.BlurWeight[cur_index].x = m_horizontal_blur_uniform_data.BlurWeight[cur_index].x = std::exp(exponent_value);
		weight_result += m_horizontal_blur_uniform_data.BlurWeight[cur_index].x;
		++cur_index;
	}


	for (uint32_t i = 0; i <= m_horizontal_blur_uniform_data.BlurWidth2; ++i)
	{
		m_horizontal_blur_uniform_data.BlurWeight[i].x /= weight_result;
		m_vertical_blur_uniform_data.BlurWeight[i].x = m_horizontal_blur_uniform_data.BlurWeight[i].x;
	}


	m_horizontal_blur_uniform_data.DirectionX = 1;
	m_horizontal_blur_uniform_data.DirectionY = 0;

	BufferUpdateDesc horizontal_blur_buffer_update_desc = {};
	horizontal_blur_buffer_update_desc.m_buffer = m_horizontal_blur_uniform_buffer;
	horizontal_blur_buffer_update_desc.m_pSource = &m_horizontal_blur_uniform_data;
	horizontal_blur_buffer_update_desc.m_size = sizeof(BlurUniformData);

	m_dxrenderer->cmd_update_buffer(horizontal_blur_buffer_update_desc);


	m_vertical_blur_uniform_data.DirectionX = 0;
	m_vertical_blur_uniform_data.DirectionY = 1;

	BufferUpdateDesc vertical_blur_buffer_update_desc = {};
	vertical_blur_buffer_update_desc.m_buffer = m_vertical_blur_uniform_buffer;
	vertical_blur_buffer_update_desc.m_pSource = &m_vertical_blur_uniform_data;
	vertical_blur_buffer_update_desc.m_size = sizeof(BlurUniformData);

	m_dxrenderer->cmd_update_buffer(vertical_blur_buffer_update_desc);

	BlurHorizontal(input_texture, intermediate_texture);
	BlurVertical(intermediate_texture, output_texture_to_blur);
}

void BlurImagePass::BlurHorizontal(Texture* input_texture, Texture* output_texture_to_blur)
{
	DescriptorData params[3] = {};
	params[0].m_binding_location = 0;
	params[0].m_buffers = &m_horizontal_blur_uniform_buffer;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[1].m_binding_location = 0;
	params[1].m_textures = &output_texture_to_blur;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
	params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[2].m_binding_location = 0;
	params[2].m_textures = &input_texture;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[2].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	m_dxrenderer->cmd_bind_descriptor(m_blur_pipeline, 3, params);
	m_dxrenderer->cmd_dispatch( (input_texture->get_desc().m_width / BLUR_WORKGROUP_THREAD_ID_X ),
		(input_texture->get_desc().m_height / BLUR_WORKGROUP_THREAD_ID_Y), BLUR_WORKGROUP_THREAD_ID_Z);
}

void BlurImagePass::BlurVertical(Texture* input_texture, Texture* output_texture_to_blur)
{
	DescriptorData params[3] = {};
	params[0].m_binding_location = 0;
	params[0].m_buffers = &m_vertical_blur_uniform_buffer;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[1].m_binding_location = 0;
	params[1].m_textures = &output_texture_to_blur;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_RW_TEXTURE;
	params[1].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	params[2].m_binding_location = 0;
	params[2].m_textures = &input_texture;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[2].m_shader_stages = Shader_Stages::COMPUTE_STAGE;

	m_dxrenderer->cmd_bind_descriptor(m_blur_pipeline, 3, params);
	m_dxrenderer->cmd_dispatch((input_texture->get_desc().m_width / BLUR_WORKGROUP_THREAD_ID_X),
		(input_texture->get_desc().m_height / BLUR_WORKGROUP_THREAD_ID_Y), BLUR_WORKGROUP_THREAD_ID_Z);
}

void BlurImagePass::LoadContent(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	char buff[100];
	snprintf(buff, sizeof(buff), "%d", m_blur_channel_type);
	std::string sample_count_definition = buff;

	ShaderMacro blur_channel_macro = { "BLUR_CHANNEL_TYPE", sample_count_definition };

	ShaderLoadDesc blur_filter_shader_load_desc = {};
	blur_filter_shader_load_desc.m_desc.m_compute_shader_path = "blur_filter_comp.hlsl";
	blur_filter_shader_load_desc.m_shader_macro = &blur_channel_macro;
	blur_filter_shader_load_desc.m_shader_macro_count = 1;

	m_blur_shader = DXResourceLoader::Create_Shader(m_dxrenderer, blur_filter_shader_load_desc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::COMPUTE;
	pipeline_desc.m_compute_desc = {};
	pipeline_desc.m_compute_desc.m_shader = m_blur_shader;

	m_blur_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
	

	BlurUniformData blur_uniform_data = {};
	blur_uniform_data.BlurWidth = (float)m_blur_width;
	blur_uniform_data.BlurWidth2 = (float)m_blur_width * 2;
	

	uint32_t s_val = m_blur_width / 2;

	float weight_result = 0;
	uint32_t cur_index = 0;
	for (int32_t i = (int32_t)(-blur_uniform_data.BlurWidth); i <= (int32_t)(blur_uniform_data.BlurWidth); ++i)
	{
		float i_over_s = (static_cast<float>(i) / static_cast<float>(s_val));
		float i_over_s_power2 = std::pow(i_over_s, 2.f);
		float exponent_value = -0.5f * i_over_s_power2;
		blur_uniform_data.BlurWeight[cur_index].x = std::exp(exponent_value);
		weight_result += blur_uniform_data.BlurWeight[cur_index].x;
		++cur_index;
	}


	for (uint32_t i = 0; i <= blur_uniform_data.BlurWidth2; ++i)
	{
		blur_uniform_data.BlurWeight[i].x /= weight_result;
	}

	BufferLoadDesc blur_buffer_uniform_desc = {};
	blur_buffer_uniform_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	blur_buffer_uniform_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	blur_buffer_uniform_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	blur_buffer_uniform_desc.m_size = sizeof(BlurUniformData);
	blur_buffer_uniform_desc.m_rawData = &blur_uniform_data;

	blur_uniform_data.DirectionX = 1;
	blur_uniform_data.DirectionY = 0;
	m_horizontal_blur_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, blur_buffer_uniform_desc);

	blur_uniform_data.DirectionX = 0;
	blur_uniform_data.DirectionY = 1;
	m_vertical_blur_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, blur_buffer_uniform_desc);
}