#include "stdafx.h"
#include "DeferredRenderingInstance.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include "Managers/ResourceManager.h"


DeferredRenderingInstance::DeferredRenderingInstance(DeferredRendering& deferredRendering)
	:m_deferredRendering(deferredRendering)
{
	m_appRenderer = m_deferredRendering.m_appRenderer;
	m_dxrenderer = m_deferredRendering.m_dxrenderer;
}


DeferredRenderingInstance::~DeferredRenderingInstance()
{
}



void DeferredRenderingInstance::Render(const AppRendererContext& appRendererContext)
{
	RenderDeferredPass(appRendererContext);
	RenderDeferredGlobalDirectionalLightShade(appRendererContext);
	RenderDeferredPointLightShade(appRendererContext);
	RenderHaloEffect(appRendererContext);
}

void DeferredRenderingInstance::Release()
{
	for (uint32_t i = 0; i < DEFERRED_TOTAL_COUNT; ++i)
	{
		SafeReleaseDelete(m_deferred_rts[i]);
	}

}

void DeferredRenderingInstance::Initialize(const AppRendererContext& appRendererContext)
{
}

void DeferredRenderingInstance::LoadContent(const AppRendererContext& appRendererContext)
{
	RenderTargetDesc rt_desc[DEFERRED_RT_TYPE::DEFERRED_TOTAL_COUNT] = {};

	DXRenderer* dxrenderer = m_deferredRendering.m_appRenderer->GetDXRenderer();

	RenderTarget* curMainRT = appRendererContext.m_appRendererInstance->m_curMainRT;

	int32_t rt_width, rt_height;

	rt_width = curMainRT->get_texture()->GetWidth();
	rt_height = curMainRT->get_texture()->GetHeight();

	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_clearVal = ClearValue{ 0.0, 0.0, 0.0, 0.0 };
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_width = rt_width;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_height = rt_height;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_depth = 1;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_mipLevels = 1;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_is_srgb = false;
	rt_desc[DEFERRED_WORLD_NORMAL].m_texture_desc.m_sampleCount = (SampleCount)GraphicsSettings::MSAA_SAMPLE_COUNT;

	m_deferred_rts[DEFERRED_WORLD_NORMAL] = DXResourceLoader::Create_RenderTarget(
		dxrenderer, rt_desc[DEFERRED_WORLD_NORMAL]);


	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_clearVal = ClearValue{ 0.0, 0.0, 0.0, 0.0 };
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_width = rt_width;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_height = rt_height;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_depth = 1;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_mipLevels = 1;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_is_srgb = false;
	rt_desc[DEFERRED_ALBEDO].m_texture_desc.m_sampleCount = (SampleCount)GraphicsSettings::MSAA_SAMPLE_COUNT;

	m_deferred_rts[DEFERRED_ALBEDO] = DXResourceLoader::Create_RenderTarget(
		dxrenderer, rt_desc[DEFERRED_ALBEDO]);

	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_clearVal = ClearValue{ 0.0, 0.0, 0.0, 0.0 };
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_width = rt_width;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_height = rt_height;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_depth = 1;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_mipLevels = 1;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_is_srgb = false;
	rt_desc[DEFERRED_SPECULAR].m_texture_desc.m_sampleCount = (SampleCount)GraphicsSettings::MSAA_SAMPLE_COUNT;

	m_deferred_rts[DEFERRED_SPECULAR] = DXResourceLoader::Create_RenderTarget(
		dxrenderer, rt_desc[DEFERRED_SPECULAR]);

	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_clearVal = ClearValue{ 0.0, 0.0, 0.0, 0.0 };
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_width = rt_width;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_height = rt_height;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_depth = 1;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_mipLevels = 1;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_imageFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_is_srgb = false;
	rt_desc[DEFERRED_STRUCTURED_BUFFER].m_texture_desc.m_sampleCount = (SampleCount)GraphicsSettings::MSAA_SAMPLE_COUNT;

	m_deferred_rts[DEFERRED_STRUCTURED_BUFFER] = DXResourceLoader::Create_RenderTarget(
		m_dxrenderer, rt_desc[DEFERRED_STRUCTURED_BUFFER]);
}

void DeferredRenderingInstance::RenderDeferredPass(const AppRendererContext& appRendererContext)
{
	LoadActionsDesc load_actions_desc = {};
	for (uint32_t i = 0; i < DEFERRED_TOTAL_COUNT; ++i)
	{
		load_actions_desc.m_clear_color_values[i] = m_deferred_rts[i]->get_clear_value();
		load_actions_desc.m_load_actions_color[i] = LoadActionType::CLEAR;
	}
	load_actions_desc.m_clear_depth_stencil = appRendererContext.m_appRendererInstance->m_depthRT->get_clear_value();
	load_actions_desc.m_load_action_depth = LoadActionType::CLEAR;

	m_dxrenderer->cmd_bind_render_targets(m_deferred_rts, DEFERRED_TOTAL_COUNT,
		appRendererContext.m_appRendererInstance->m_depthRT, load_actions_desc);

	m_dxrenderer->cmd_set_viewport(0, 0, m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_desc().m_texture_desc.m_width,
		m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_desc().m_texture_desc.m_height);

	appRendererContext.m_appRendererInstance->RenderBasicInstances(m_deferredRendering.m_deferred_pass_pipeline);
}

void DeferredRenderingInstance::RenderDeferredGlobalDirectionalLightShade(const AppRendererContext& appRendererContext)
{
	AppRendererInstance* appRendererInstance = appRendererContext.m_appRendererInstance;
	RenderTarget* curMainRT = appRendererContext.m_appRendererInstance->m_curMainRT;

	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_clear_color_values[0] = curMainRT->get_clear_value();
	load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;
	load_actions_desc.m_clear_depth_stencil = appRendererInstance->m_depthRT->get_clear_value();
	load_actions_desc.m_load_action_depth = LoadActionType::DONT_CLEAR;

	m_dxrenderer->cmd_bind_render_targets(&appRendererContext.m_appRendererInstance->m_curMainRT, 1, nullptr, load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, curMainRT->get_desc().m_texture_desc.m_width,
		curMainRT->get_desc().m_texture_desc.m_height);

	m_dxrenderer->cmd_bind_pipeline(m_deferredRendering.m_deferred_shade_pipeline);

	Texture* world_normal_texture = m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_texture();
	Texture* albedo_texture = m_deferred_rts[DEFERRED_ALBEDO]->get_texture();
	Texture* specular_texture = m_deferred_rts[DEFERRED_SPECULAR]->get_texture();
	Texture* depth_texture = appRendererContext.m_appRendererInstance->m_depthRT->get_texture();

	Buffer* lightUniformBuffer = m_appRenderer->m_momentShadowMapRendering.get_light_camera_uniform_buffer();
	Texture* shadowmapTexture = m_appRenderer->m_momentShadowMapRendering.get_blurred_shadow_map_texture();

	DescriptorData params[10] = {};

	params[0].m_binding_location = 0;
	params[0].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
	params[0].m_samplers = &m_appRenderer->m_clamp_linear_sampler;

	params[1].m_binding_location = 0;
	params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[1].m_textures = &world_normal_texture;

	params[2].m_binding_location = 1;
	params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[2].m_textures = &albedo_texture;

	params[3].m_binding_location = 2;
	params[3].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[3].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[3].m_textures = &specular_texture;

	params[4].m_binding_location = 3;
	params[4].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[4].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[4].m_textures = &depth_texture;

	params[5].m_binding_location = 0;
	params[5].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[5].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[5].m_buffers = &appRendererContext.m_appRendererInstance->m_camera_uniform_buffer;

	params[6].m_binding_location = 1;
	params[6].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[6].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[6].m_buffers = &m_appRenderer->m_directional_light_uniform_buffer;

	params[7].m_binding_location = 2;
	params[7].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[7].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[7].m_buffers = &lightUniformBuffer;

	params[8].m_binding_location = 4;
	params[8].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[8].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[8].m_textures = &shadowmapTexture;

	m_dxrenderer->cmd_bind_descriptor(m_deferredRendering.m_deferred_shade_pipeline, 9, params);
	m_dxrenderer->cmd_draw(6, 0);
}

void DeferredRenderingInstance::RenderDeferredPointLightShade(const AppRendererContext& appRendererContext)
{
	uint32_t point_light_inst_count = static_cast<uint32_t>(m_appRenderer->m_pointLightInstanceList.size());

	if (point_light_inst_count == 0)
	{
		return;
	}

	m_dxrenderer->cmd_bind_pipeline(m_deferredRendering.m_deferred_shade_pointlight_pipeline);

	Model* sphere_model = m_deferredRendering.m_resourceManager->GetModel(StringId("Assets/Models/Sphere.fbx"));

	m_dxrenderer->cmd_bind_vertex_buffer(sphere_model->get_vertex_buffer());
	m_dxrenderer->cmd_bind_index_buffer(sphere_model->get_index_buffer());

	Texture* world_normal_texture = m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_texture();
	Texture* albedo_texture = m_deferred_rts[DEFERRED_ALBEDO]->get_texture();
	Texture* specular_texture = m_deferred_rts[DEFERRED_SPECULAR]->get_texture();
	Texture* depth_texture = appRendererContext.m_appRendererInstance->m_depthRT->get_texture();

	DescriptorData params[7] = {};

	params[0].m_binding_location = 1;
	params[0].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[0].m_textures = &world_normal_texture;

	params[1].m_binding_location = 2;
	params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[1].m_textures = &albedo_texture;

	params[2].m_binding_location = 3;
	params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[2].m_textures = &specular_texture;

	params[3].m_binding_location = 4;
	params[3].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[3].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[3].m_textures = &depth_texture;

	params[4].m_binding_location = 0;
	params[4].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[4].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[4].m_buffers = &appRendererContext.m_appRendererInstance->m_camera_uniform_buffer;

	params[5].m_binding_location = 0;
	params[5].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[5].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[5].m_buffers = &m_deferredRendering.m_point_light_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_deferredRendering.m_deferred_shade_pointlight_pipeline, 6, params);
	m_dxrenderer->cmd_draw_index_instanced(point_light_inst_count, 0, sphere_model->get_index_total_count(), 0, 0);
}


void DeferredRenderingInstance::RenderHaloEffect(const AppRendererContext& appRenderereContext)
{
	uint32_t haloEffectInstCount = static_cast<uint32_t>(m_appRenderer->m_haloEffectInstanceList.size());

	if (haloEffectInstCount == 0)
	{
		return;
	}

	m_dxrenderer->cmd_bind_pipeline(m_deferredRendering.m_shadeHaloEffectPipeline);

	Model* sphere_model = m_deferredRendering.m_resourceManager->GetModel(StringId("Assets/Models/Sphere.fbx"));

	m_dxrenderer->cmd_bind_vertex_buffer(sphere_model->get_vertex_buffer());
	m_dxrenderer->cmd_bind_index_buffer(sphere_model->get_index_buffer());

	Texture* structureBufferTexture = m_deferred_rts[DEFERRED_STRUCTURED_BUFFER]->get_texture();
	DescriptorData params[4] = {};

	params[0].m_binding_location = 0;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_buffers = &m_deferredRendering.m_haloEffectUniformBuffer;

	params[1].m_binding_location = 0;
	params[1].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[1].m_buffers = &appRenderereContext.m_appRendererInstance->m_camera_uniform_buffer;

	params[2].m_binding_location = 1;
	params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[2].m_textures = &structureBufferTexture;

	m_dxrenderer->cmd_bind_descriptor(m_deferredRendering.m_shadeHaloEffectPipeline, 3, params);
	m_dxrenderer->cmd_draw_index_instanced(haloEffectInstCount, 0, sphere_model->get_index_total_count(), 0, 0);
}