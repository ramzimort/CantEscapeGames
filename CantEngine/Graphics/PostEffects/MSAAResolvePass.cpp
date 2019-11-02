#include "stdafx.h"
#include "MSAAResolvePass.h"
#include "Graphics/AppRenderer.h"
#include "Shaders/Shading.h"
#include "Graphics/PostEffects/MSAAResolvePass.h"
#include "Graphics/GraphicsSettings.h"
#include "CantDebug/CantDebug.h"


MSAAResolvePass::MSAAResolvePass(AppRenderer* app_renderer)
	:m_appRenderer(app_renderer)
{
}


MSAAResolvePass::~MSAAResolvePass()
{
}

void MSAAResolvePass::LoadContent(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	ShaderMacro msaa_sample_macro = { "SAMPLE_COUNT", std::to_string(GraphicsSettings::MSAA_SAMPLE_COUNT) };

	ShaderLoadDesc msaa_resolve_shader_desc = {};
	msaa_resolve_shader_desc.m_desc.m_vertex_shader_path = "msaa_resolve_vert.hlsl";
	msaa_resolve_shader_desc.m_desc.m_pixel_shader_path = "msaa_resolve_frag.hlsl";
	msaa_resolve_shader_desc.m_shader_macro = &msaa_sample_macro;
	msaa_resolve_shader_desc.m_shader_macro_count = 1;

	m_resolve_shader = DXResourceLoader::Create_Shader(m_dxrenderer, msaa_resolve_shader_desc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;
	pipeline_desc.m_graphics_desc = {};

	GraphicsPipelineDesc& msaa_resolve_pipeline_desc = pipeline_desc.m_graphics_desc;
	msaa_resolve_pipeline_desc.m_render_target_count = 1;
	msaa_resolve_pipeline_desc.m_depth_state = m_appRenderer->m_less_equal_depth_state;
	msaa_resolve_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	msaa_resolve_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_state;
	msaa_resolve_pipeline_desc.m_shader = m_resolve_shader;

	m_resolve_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

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


void MSAAResolvePass::Release()
{
	SafeReleaseDelete(m_msaa_resolve_uniform_buffer);
	SafeReleaseDelete(m_resolve_pipeline);
	SafeReleaseDelete(m_resolve_shader);
}