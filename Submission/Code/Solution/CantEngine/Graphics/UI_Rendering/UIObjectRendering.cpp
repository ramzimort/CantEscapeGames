#include "stdafx.h"
#include "UIObjectRendering.h"
#include "Graphics/AppRenderer.h"

UIObjectRendering::UIObjectRendering(AppRenderer* appRenderer)
	:m_appRenderer(appRenderer)
{
}


UIObjectRendering::~UIObjectRendering()
{
}


void UIObjectRendering::Initialize(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	VertexLayout pos_normal_tangent_bitangent_uv_layout = {};
	pos_normal_tangent_bitangent_uv_layout.m_atrrib_count = 5;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[0].m_binding = 0;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;

	pos_normal_tangent_bitangent_uv_layout.m_attribs[1].m_binding = 0;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[1].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[1].m_semantic = Attrib_Semantic::NORMAL;

	pos_normal_tangent_bitangent_uv_layout.m_attribs[2].m_binding = 0;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[2].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[2].m_semantic = Attrib_Semantic::TANGENT;

	pos_normal_tangent_bitangent_uv_layout.m_attribs[3].m_binding = 0;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[3].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[3].m_semantic = Attrib_Semantic::BITANGENT;

	pos_normal_tangent_bitangent_uv_layout.m_attribs[4].m_binding = 0;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[4].m_format = DXGI_FORMAT_R32G32_FLOAT;
	pos_normal_tangent_bitangent_uv_layout.m_attribs[4].m_semantic = Attrib_Semantic::TEXCOORD_0;

	ShaderLoadDesc uiObjectRenderingShaderDesc = {};
	uiObjectRenderingShaderDesc.m_desc.m_vertex_shader_path = "ui_quad_vert.hlsl";
	uiObjectRenderingShaderDesc.m_desc.m_pixel_shader_path = "ui_quad_frag.hlsl";

	m_pUIObjectRenderingShader = DXResourceLoader::Create_Shader(m_dxrenderer, uiObjectRenderingShaderDesc);

	PipelineDesc pipelineDesc = {};
	pipelineDesc.m_graphics_desc = {};
	pipelineDesc.m_pipeline_type = PipelineType::GRAPHICS;

	GraphicsPipelineDesc& uiObjectRenderingPipelineDesc = pipelineDesc.m_graphics_desc;
	uiObjectRenderingPipelineDesc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	uiObjectRenderingPipelineDesc.m_depth_state = m_appRenderer->m_disabled_depth_state;
	uiObjectRenderingPipelineDesc.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_state;
	uiObjectRenderingPipelineDesc.m_vertex_layout = &pos_normal_tangent_bitangent_uv_layout;
	uiObjectRenderingPipelineDesc.m_render_target_count = 1;
	uiObjectRenderingPipelineDesc.m_shader = m_pUIObjectRenderingShader;
	uiObjectRenderingPipelineDesc.m_blend_state = m_appRenderer->m_uiTransparencyBlending;
	
	m_pUIObjectRenderingPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipelineDesc);

}

void UIObjectRendering::Release()
{
	SafeReleaseDelete(m_pUIObjectRenderingPipeline);
	SafeReleaseDelete(m_pUIObjectRenderingShader);
}