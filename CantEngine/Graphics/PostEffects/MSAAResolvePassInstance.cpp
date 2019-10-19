#include "stdafx.h"
#include "MSAAResolvePassInstance.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"


MSAAResolvePassInstance::MSAAResolvePassInstance(MSAAResolvePass& msaaResolvePass)
	:m_msaaResolvePass(msaaResolvePass)
{
	m_appRenderer = m_msaaResolvePass.m_appRenderer;
	m_dxrenderer = m_msaaResolvePass.m_dxrenderer;
}


MSAAResolvePassInstance::~MSAAResolvePassInstance()
{
}


void MSAAResolvePassInstance::Release()
{
	SafeReleaseDelete(m_msaa_resolve_uniform_buffer);
}

void MSAAResolvePassInstance::Render(const AppRendererContext& appRendererContext)
{
	RenderTarget* outputRT = appRendererContext.m_appRendererInstance->m_finalOutputRT;

	LoadActionsDesc load_action_desc = {};
	load_action_desc.m_load_actions_color[0] = LoadActionType::CLEAR;
	load_action_desc.m_clear_color_values[0] = outputRT->get_clear_value();
	load_action_desc.m_load_action_depth = LoadActionType::DONT_CLEAR;
	load_action_desc.m_load_action_stencil = LoadActionType::DONT_CLEAR;


	m_dxrenderer->cmd_bind_render_targets(&appRendererContext.m_appRendererInstance->m_finalOutputRT, 1, nullptr, load_action_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, outputRT->get_desc().m_texture_desc.m_width,
		outputRT->get_desc().m_texture_desc.m_height);

	m_msaa_resolve_uniform_data.FilterSize = GraphicsSettings::MSAA_Filter_Size;
	m_msaa_resolve_uniform_data.SampleRadius = static_cast<int> ((m_msaa_resolve_uniform_data.FilterSize / 2.0f) + 0.499f);

	BufferUpdateDesc update_msaa_uniform_buffer_desc = {};
	update_msaa_uniform_buffer_desc.m_buffer = m_msaa_resolve_uniform_buffer;
	update_msaa_uniform_buffer_desc.m_pSource = &m_msaa_resolve_uniform_data;
	update_msaa_uniform_buffer_desc.m_size = sizeof(MSAAResolveUniformData);
	m_dxrenderer->cmd_update_buffer(update_msaa_uniform_buffer_desc);


	m_dxrenderer->cmd_bind_pipeline(m_msaaResolvePass.m_resolve_pipeline);

	Texture* cur_main_rt_texture = appRendererContext.m_appRendererInstance->m_curMainRT->get_texture();

	DescriptorData params[3] = {};
	params[0].m_binding_location = 0;
	params[0].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[0].m_textures = &cur_main_rt_texture;

	params[1].m_binding_location = 0;
	params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[1].m_buffers = &appRendererContext.m_appRendererInstance->m_camera_uniform_buffer;

	params[2].m_binding_location = 1;
	params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[2].m_buffers = &m_msaa_resolve_uniform_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_msaaResolvePass.m_resolve_pipeline, 3, params);
	m_dxrenderer->cmd_draw(3, 0);
}

void MSAAResolvePassInstance::Initialize(const AppRendererContext& appRendererContext)
{
	BufferLoadDesc msaa_uniform_buffer_desc = {};
	msaa_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	msaa_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	msaa_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	msaa_uniform_buffer_desc.m_desc.m_debugName = "MSAA_UNIFORM_BUFFER";
	msaa_uniform_buffer_desc.m_size = sizeof(MSAAResolveUniformData);
	msaa_uniform_buffer_desc.m_rawData = nullptr;

	m_msaa_resolve_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, msaa_uniform_buffer_desc);


	if (GraphicsSettings::MSAA_SAMPLE_COUNT == 8)
	{
		m_msaa_resolve_uniform_data.SampleOffsets[0] = Vector4(0.580f, 0.298f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[1] = Vector4(0.419f, 0.698f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[2] = Vector4(0.819f, 0.580f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[3] = Vector4(0.298f, 0.180f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[4] = Vector4(0.180f, 0.819f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[5] = Vector4(0.058f, 0.419f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[6] = Vector4(0.698f, 0.941f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[7] = Vector4(0.941f, 0.058f, 0.0f, 0.0f);
	}
	else if (GraphicsSettings::MSAA_SAMPLE_COUNT == 4)
	{
		m_msaa_resolve_uniform_data.SampleOffsets[0] = Vector4(0.380f, 0.141f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[1] = Vector4(0.859f, 0.380f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[2] = Vector4(0.141f, 0.620f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[3] = Vector4(0.619f, 0.859f, 0.0f, 0.0f);
	}
	else if (GraphicsSettings::MSAA_SAMPLE_COUNT == 2)
	{
		m_msaa_resolve_uniform_data.SampleOffsets[0] = Vector4(0.741f, 0.741f, 0.0f, 0.0f);
		m_msaa_resolve_uniform_data.SampleOffsets[1] = Vector4(0.258f, 0.258f, 0.0f, 0.0f);
	}

	for (uint32_t i = 0; i < 8; ++i)
	{
		m_msaa_resolve_uniform_data.SampleOffsets[i] -= Vector4(0.5f, 0.5f, 0.0f, 0.0f);
	}

	m_msaa_resolve_uniform_data.FilterSize = 2.5f;

	m_msaa_resolve_uniform_data.SampleRadius = static_cast<int> ((m_msaa_resolve_uniform_data.FilterSize / 2.0f) + 0.499f);
}
void MSAAResolvePassInstance::LoadContent(const AppRendererContext& appRendererContext)
{

}