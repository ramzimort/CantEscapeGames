#include "stdafx.h"
#include "DeferredRendering.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include "Managers/ResourceManager.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/PostEffects/HaloEffect.h"


DeferredRendering::DeferredRendering(AppRenderer* app_renderer, ResourceManager* resourceManager)
	:m_appRenderer(app_renderer),
	m_resourceManager(resourceManager)

{
}


DeferredRendering::~DeferredRendering()
{
}


void DeferredRendering::Release()
{
	SafeReleaseDelete(m_deferred_pass_pipeline);
	SafeReleaseDelete(m_deferred_pass_shader);

	SafeReleaseDelete(m_deferredBonePassPipeline);
	SafeReleaseDelete(m_deferredBonePassShader);

	SafeReleaseDelete(m_deferred_shade_pipeline);
	SafeReleaseDelete(m_deferred_shade_shader);
	SafeReleaseDelete(m_shadeHaloEffectShader);
	SafeReleaseDelete(m_shadeHaloEffectPipeline);
	SafeReleaseDelete(m_haloEffectUniformBuffer);


	SafeReleaseDelete(m_deferred_shade_pointlight_pipeline);
	SafeReleaseDelete(m_deferred_shade_pointlight_shader);
	SafeReleaseDelete(m_point_light_buffer);
}


void DeferredRendering::LoadContent(DXRenderer* dxrenderer)
{
	char buff[100];
	snprintf(buff, sizeof(buff), "%d", GraphicsSettings::MSAA_SAMPLE_COUNT);
	std::string sample_count_definition = buff;

	ShaderMacro msaa_sample_macro = { "SAMPLE_COUNT", sample_count_definition };

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


	VertexLayout pos_normal_tangent_bitangent_uv_bones_layout = {};
	pos_normal_tangent_bitangent_uv_bones_layout.m_atrrib_count = 7;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_semantic = Attrib_Semantic::NORMAL;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_semantic = Attrib_Semantic::TANGENT;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_semantic = Attrib_Semantic::BITANGENT;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_format = DXGI_FORMAT_R32G32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_semantic = Attrib_Semantic::TEXCOORD_0;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_format = DXGI_FORMAT_R32G32B32A32_SINT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_semantic = Attrib_Semantic::TEXCOORD_1;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_semantic = Attrib_Semantic::TEXCOORD_2;


	VertexLayout pos_layout = {};
	pos_layout.m_atrrib_count = 1;
	pos_layout.m_attribs[0].m_binding = 0;
	pos_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;


	RenderTarget* swap_chain_rt = m_dxrenderer->GetSwapChain()->m_p_swap_chain_render_target;

	swap_chain_rt->get_desc().m_texture_desc.m_width;

	int32_t rt_width, rt_height;

	rt_width = swap_chain_rt->get_desc().m_texture_desc.m_width;
	rt_height = swap_chain_rt->get_desc().m_texture_desc.m_height;


	ShaderMacro deferredPassShaderMacro = {};
	deferredPassShaderMacro.m_name = "ANIM_MODEL";
	deferredPassShaderMacro.m_definition = std::to_string(0);

	ShaderLoadDesc deferred_pass_shader_load_desc = {};
	deferred_pass_shader_load_desc.m_desc.m_vertex_shader_path = "deferred_pass_vert.hlsl";
	deferred_pass_shader_load_desc.m_desc.m_pixel_shader_path = "deferred_pass_frag.hlsl";

	deferred_pass_shader_load_desc.m_shader_macro_count = 1;
	deferred_pass_shader_load_desc.m_shader_macro = &deferredPassShaderMacro;

	m_deferred_pass_shader = DXResourceLoader::Create_Shader(m_dxrenderer,
		deferred_pass_shader_load_desc);


	deferredPassShaderMacro.m_definition = std::to_string(1);
	deferred_pass_shader_load_desc.m_shader_macro = &deferredPassShaderMacro;
	

	m_deferredBonePassShader = DXResourceLoader::Create_Shader(m_dxrenderer, deferred_pass_shader_load_desc);


	ShaderLoadDesc deferred_shade_shader_load_desc = {};
	deferred_shade_shader_load_desc.m_desc.m_vertex_shader_path = "fullscreen_quad_vert.hlsl";
	deferred_shade_shader_load_desc.m_desc.m_pixel_shader_path = "deferred_shade_frag.hlsl";
	deferred_shade_shader_load_desc.m_shader_macro_count = 1;
	deferred_shade_shader_load_desc.m_shader_macro = &msaa_sample_macro;
	m_deferred_shade_shader = DXResourceLoader::Create_Shader(m_dxrenderer,
		deferred_shade_shader_load_desc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;
	pipeline_desc.m_graphics_desc = {};

	GraphicsPipelineDesc& graphic_pipeline_desc = pipeline_desc.m_graphics_desc;
	graphic_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphic_pipeline_desc.m_render_target_count = DEFERRED_TOTAL_COUNT;
	if (GraphicsSettings::MSAA_SAMPLE_COUNT > 1)
	{
		graphic_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_front_rasterizer_ms_state;
	}
	else
	{
		graphic_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_front_rasterizer_state;
	}
	graphic_pipeline_desc.m_depth_state = m_appRenderer->m_less_equal_depth_state;
	graphic_pipeline_desc.m_vertex_layout = &pos_normal_tangent_bitangent_uv_layout;
	graphic_pipeline_desc.m_shader = m_deferred_pass_shader;

	m_deferred_pass_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	graphic_pipeline_desc.m_vertex_layout = &pos_normal_tangent_bitangent_uv_bones_layout;
	graphic_pipeline_desc.m_shader = m_deferredBonePassShader;

	m_deferredBonePassPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	graphic_pipeline_desc = {};

	graphic_pipeline_desc.m_render_target_count = 1;
	graphic_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphic_pipeline_desc.m_depth_state = m_appRenderer->m_disabled_depth_state;
	graphic_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_state;
	graphic_pipeline_desc.m_shader = m_deferred_shade_shader;
	graphic_pipeline_desc.m_vertex_layout = nullptr;

	m_deferred_shade_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	ShaderLoadDesc deferred_shade_pointlight_shader_desc = {};
	deferred_shade_pointlight_shader_desc.m_desc.m_vertex_shader_path = "deferred_shade_pointlight_vert.hlsl";
	deferred_shade_pointlight_shader_desc.m_desc.m_pixel_shader_path = "deferred_shade_pointlight_frag.hlsl";
	deferred_shade_pointlight_shader_desc.m_shader_macro_count = 1;
	deferred_shade_pointlight_shader_desc.m_shader_macro = &msaa_sample_macro;

	m_deferred_shade_pointlight_shader = DXResourceLoader::Create_Shader(m_dxrenderer, deferred_shade_pointlight_shader_desc);


	graphic_pipeline_desc = {};
	graphic_pipeline_desc.m_render_target_count = 1;
	graphic_pipeline_desc.m_shader = m_deferred_shade_pointlight_shader;
	graphic_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphic_pipeline_desc.m_vertex_layout = &pos_layout;
	graphic_pipeline_desc.m_depth_state = m_appRenderer->m_disabled_depth_state;
	graphic_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_back_rasterizer_state;
	graphic_pipeline_desc.m_blend_state = m_appRenderer->m_blend_state_one_zero_add;

	m_deferred_shade_pointlight_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	ShaderLoadDesc shadeHaloEffectShaderDesc = {};
	shadeHaloEffectShaderDesc.m_desc.m_vertex_shader_path = "shade_halo_effect_vert.hlsl";
	shadeHaloEffectShaderDesc.m_desc.m_pixel_shader_path = "shade_halo_effect_frag.hlsl";

	m_shadeHaloEffectShader = DXResourceLoader::Create_Shader(m_dxrenderer, shadeHaloEffectShaderDesc);

	graphic_pipeline_desc = {};
	graphic_pipeline_desc.m_render_target_count = 1;
	graphic_pipeline_desc.m_shader = m_shadeHaloEffectShader;
	graphic_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphic_pipeline_desc.m_vertex_layout = &pos_layout;
	graphic_pipeline_desc.m_depth_state = m_appRenderer->m_disabled_depth_state;
	graphic_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_back_rasterizer_state;
	graphic_pipeline_desc.m_blend_state = m_appRenderer->m_additiveBlending;

	m_shadeHaloEffectPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	BufferLoadDesc haloEffectUniformBufferDesc = {};
	haloEffectUniformBufferDesc.m_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	haloEffectUniformBufferDesc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	haloEffectUniformBufferDesc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	haloEffectUniformBufferDesc.m_desc.m_miscFlags = Misc_Flags::MISC_BUFFER_STRUCTURED;
	haloEffectUniformBufferDesc.m_desc.m_elementCount = MAX_DEFERRED_HALO_EFFECT;
	haloEffectUniformBufferDesc.m_desc.m_structureStride = sizeof(ConstantHaloEffectData);
	haloEffectUniformBufferDesc.m_size = haloEffectUniformBufferDesc.m_desc.m_elementCount *
		haloEffectUniformBufferDesc.m_desc.m_structureStride;
	haloEffectUniformBufferDesc.m_rawData = nullptr;

	m_haloEffectUniformBuffer = DXResourceLoader::Create_Buffer(m_dxrenderer, haloEffectUniformBufferDesc);


	BufferLoadDesc point_light_load_buffer_desc = {};
	point_light_load_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	point_light_load_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	point_light_load_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	point_light_load_buffer_desc.m_desc.m_miscFlags = Misc_Flags::MISC_BUFFER_STRUCTURED;
	point_light_load_buffer_desc.m_desc.m_elementCount = MAX_DEFERRED_POINT_LIGHT;
	point_light_load_buffer_desc.m_desc.m_structureStride = sizeof(ConstantPointLightData);
	point_light_load_buffer_desc.m_size = point_light_load_buffer_desc.m_desc.m_elementCount *
		point_light_load_buffer_desc.m_desc.m_structureStride;
	point_light_load_buffer_desc.m_rawData = nullptr;

	m_point_light_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, point_light_load_buffer_desc);
}



void DeferredRendering::Update(float dt)
{
	
}

void DeferredRendering::UpdateUniformBuffer()
{
	uint32_t point_light_inst_count = static_cast<uint32_t>(m_appRenderer->m_pointLightInstanceList.size());

	if (point_light_inst_count > 0)
	{
		for (uint32_t i = 0; i < point_light_inst_count; ++i)
		{
			const PointLightInstanceData& light_inst = m_appRenderer->m_pointLightInstanceList[i];
			const Light* cur_light = light_inst.light;

			Vector4 light_color(cur_light->GetColor().x, cur_light->GetColor().y, cur_light->GetColor().z, 1.f);

			m_constant_point_light_data[i].LightColor = light_color;
			m_constant_point_light_data[i].LightPosition = Vector4(light_inst.light_position.x,
				light_inst.light_position.y, light_inst.light_position.z, 1.0);
			m_constant_point_light_data[i].LightMiscData.x = cur_light->GetRadius();
			m_constant_point_light_data[i].LightMiscData.y = cur_light->GetIntensity();

			Vector2 attenuation_constant = cur_light->GetAttenuationConstant();
			m_constant_point_light_data[i].LightMiscData.z = attenuation_constant.x;
			m_constant_point_light_data[i].LightMiscData.w = attenuation_constant.y;
		}
		BufferUpdateDesc update_constant_point_light_buffer_desc = {};
		update_constant_point_light_buffer_desc.m_buffer = m_point_light_buffer;
		update_constant_point_light_buffer_desc.m_pSource = &m_constant_point_light_data;
		update_constant_point_light_buffer_desc.m_size = static_cast<uint32_t>(sizeof(ConstantPointLightData) *
			m_appRenderer->m_pointLightInstanceList.size());

		m_dxrenderer->cmd_update_buffer(update_constant_point_light_buffer_desc);
	}


	uint32_t haloEffectInstCount = static_cast<uint32_t>(m_appRenderer->m_haloEffectInstanceList.size());
	if (haloEffectInstCount > 0)
	{
		for (uint32_t i = 0; i < haloEffectInstCount; ++i)
		{
			const HaloEffectInstanceData& haloEffectInst = m_appRenderer->m_haloEffectInstanceList[i];
			const HaloEffect* curHaloEffect = haloEffectInst.haloEffect;

			m_constantHaloEffectLightData[i].HaloColor = MathUtil::v3_to_v4( curHaloEffect->GetColor() * curHaloEffect->GetIntensity(), 1.f);
			m_constantHaloEffectLightData[i].HaloPosition = MathUtil::v3_to_v4(haloEffectInst.m_haloPosition, 1.f);

			float haloEffectRadius = curHaloEffect->GetRadius();
			float radiusPow2 = haloEffectRadius * haloEffectRadius;

			m_constantHaloEffectLightData[i].HaloMiscData.x = radiusPow2;
			m_constantHaloEffectLightData[i].HaloMiscData.y = 1.f / radiusPow2;
			m_constantHaloEffectLightData[i].HaloMiscData.z = 1.f / (3.f * radiusPow2);
			m_constantHaloEffectLightData[i].HaloMiscData.w = 3.f / (4.f * haloEffectRadius);

			m_constantHaloEffectLightData[i].HaloMiscData2.x = haloEffectRadius;
		}

		BufferUpdateDesc updateConstantHaloEffectBufferDesc = {};
		updateConstantHaloEffectBufferDesc.m_buffer = m_haloEffectUniformBuffer;
		updateConstantHaloEffectBufferDesc.m_pSource = &m_constantHaloEffectLightData;
		updateConstantHaloEffectBufferDesc.m_size = static_cast<uint32_t>( sizeof(ConstantHaloEffectData) * haloEffectInstCount);

		m_dxrenderer->cmd_update_buffer(updateConstantHaloEffectBufferDesc);
	}
	
}

void DeferredRendering::RenderDeferredScene()
{
	
}