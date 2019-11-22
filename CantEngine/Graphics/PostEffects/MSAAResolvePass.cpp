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
}


void MSAAResolvePass::Release()
{
	SafeReleaseDelete(m_msaa_resolve_uniform_buffer);
	SafeReleaseDelete(m_resolve_pipeline);
	SafeReleaseDelete(m_resolve_shader);
}