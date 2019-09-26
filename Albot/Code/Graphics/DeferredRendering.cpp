#include "stdafx.h"
#include "DeferredRendering.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Model.h"
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include "Managers/ResourceManager.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"


DeferredRendering::DeferredRendering(AppRenderer* app_renderer, ResourceManager* resourceManager)
	:m_app_renderer(app_renderer),
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
	
	for (uint32_t i = 0; i < DEFERRED_TOTAL_COUNT; ++i)
	{
		SafeReleaseDelete(m_deferred_rts[i]);
	}

	SafeReleaseDelete(m_deferred_shade_pipeline);
	SafeReleaseDelete(m_deferred_shade_shader);

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


	RenderTargetDesc rt_desc[DEFERRED_RT_TYPE::DEFERRED_TOTAL_COUNT] = {};

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
		m_dxrenderer, rt_desc[DEFERRED_WORLD_NORMAL]);


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
		m_dxrenderer, rt_desc[DEFERRED_ALBEDO]);

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
		m_dxrenderer, rt_desc[DEFERRED_SPECULAR]);

	ShaderLoadDesc deferred_pass_shader_load_desc = {};
	deferred_pass_shader_load_desc.m_desc.m_vertex_shader_path = "deferred_pass_vert.hlsl";
	deferred_pass_shader_load_desc.m_desc.m_pixel_shader_path = "deferred_pass_frag.hlsl";

	m_deferred_pass_shader = DXResourceLoader::Create_Shader(m_dxrenderer,
		deferred_pass_shader_load_desc);

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
		graphic_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_front_rasterizer_ms_state;
	}
	else
	{
		graphic_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_front_rasterizer_state;
	}
	graphic_pipeline_desc.m_depth_state = m_app_renderer->m_less_equal_depth_state;
	graphic_pipeline_desc.m_vertex_layout = &pos_normal_tangent_bitangent_uv_layout;
	graphic_pipeline_desc.m_shader = m_deferred_pass_shader;

	m_deferred_pass_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	graphic_pipeline_desc = {};

	graphic_pipeline_desc.m_render_target_count = 1;
	graphic_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphic_pipeline_desc.m_depth_state = m_app_renderer->m_disabled_depth_state;
	graphic_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_none_rasterizer_state;
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
	graphic_pipeline_desc.m_depth_state = m_app_renderer->m_disabled_depth_state;
	graphic_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_back_rasterizer_state;
	graphic_pipeline_desc.m_blend_state = m_app_renderer->m_blend_state_one_zero_add;

	m_deferred_shade_pointlight_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


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

	init_deferred_scene_light();
}


void DeferredRendering::init_deferred_scene_light()
{
	Vector3 start_pos(-150.f, -30.f, -150.f);
	Vector3 inc_pos(10.f, 0.f, 10.f);
	for (int i = 0; i < 30; ++i)
	{
		for (int k = 0; k < 30; ++k)
		{
			Light& light = m_point_light_deferred_scene[(i * 30) + k];
			int light_color_max = 1000;
			light.m_color = Vector3((float)(rand() % light_color_max) / (float)light_color_max,
				(float)(rand() % light_color_max) / (float)light_color_max, (float)(rand() % light_color_max) / (float)light_color_max);
			light.m_radius = static_cast<float>(10 + rand() % 100);
			light.m_intensity = static_cast<float>(2 + rand() % 2);
			light.m_light_type = ELightType::POINT_LIGHT;
		}
	}
}


void DeferredRendering::RenderDeferredScene()
{
	render_deferred_pass();
	render_deferred_global_directional_light_shade();
	//render_deferred_point_light_shade();
}

void DeferredRendering::render_deferred_pass()
{
	LoadActionsDesc load_actions_desc = {};
	for (uint32_t i = 0; i < DEFERRED_TOTAL_COUNT; ++i)
	{
		load_actions_desc.m_clear_color_values[i] = m_deferred_rts[i]->get_clear_value();
		load_actions_desc.m_load_actions_color[i] = LoadActionType::CLEAR;
	}
	load_actions_desc.m_clear_depth_stencil = m_app_renderer->m_depth_rt->get_clear_value();
	load_actions_desc.m_load_action_depth = LoadActionType::CLEAR;

	m_dxrenderer->cmd_bind_render_targets(m_deferred_rts, DEFERRED_TOTAL_COUNT,
		m_app_renderer->m_depth_rt, load_actions_desc);

	m_dxrenderer->cmd_set_viewport(0, 0, m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_desc().m_texture_desc.m_width,
		m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_desc().m_texture_desc.m_height);

	m_app_renderer->RenderBasicInstances(m_deferred_pass_pipeline);
}

void DeferredRendering::render_deferred_global_directional_light_shade()
{
	//RenderTarget* swap_chain_rt = m_dxrenderer->GetSwapChain()->m_p_swap_chain_render_target;

	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_clear_color_values[0] = m_app_renderer->m_cur_main_rt->get_clear_value();
	load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;
	load_actions_desc.m_clear_depth_stencil = m_app_renderer->m_depth_rt->get_clear_value();
	load_actions_desc.m_load_action_depth = LoadActionType::DONT_CLEAR;

	m_dxrenderer->cmd_bind_render_targets(&m_app_renderer->m_cur_main_rt, 1, nullptr, load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, m_app_renderer->m_cur_main_rt->get_desc().m_texture_desc.m_width,
		m_app_renderer->m_cur_main_rt->get_desc().m_texture_desc.m_height);

	m_dxrenderer->cmd_bind_pipeline(m_deferred_shade_pipeline);

	Texture* world_normal_texture = m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_texture();
	Texture* albedo_texture = m_deferred_rts[DEFERRED_ALBEDO]->get_texture();
	Texture* specular_texture = m_deferred_rts[DEFERRED_SPECULAR]->get_texture();
	Texture* depth_texture = m_app_renderer->m_depth_rt->get_texture();


	DescriptorData params[10] = {};

	params[0].m_binding_location = 0;
	params[0].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
	params[0].m_samplers = &m_app_renderer->m_clamp_linear_sampler;

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
	params[5].m_buffers = &m_app_renderer->m_camera_uniform_buffer;

	params[6].m_binding_location = 1;
	params[6].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[6].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[6].m_buffers = &m_app_renderer->m_directional_light_uniform_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_deferred_shade_pipeline, 7, params);
	m_dxrenderer->cmd_draw(6, 0);
}

void DeferredRendering::render_deferred_point_light_shade()
{
	uint32_t point_light_inst_count = static_cast<uint32_t>(m_app_renderer->m_point_light_instance_list.size());
	if (point_light_inst_count == 0)
	{
		return;
	}

	Vector3 start_pos(-150.f, 10.f, -220.f);
	Vector3 inc_pos(10.f, 0.f, 30.f);
	for (int i = 0; i < 30; ++i)
	{
		for (int k = 0; k < 30; ++k)
		{
			Light& light = m_point_light_deferred_scene[(i * 30) + k];

			PointLightInstanceData inst_data;
			inst_data.light = &light;
			inst_data.light_position = start_pos + Vector3(inc_pos.x * k, 0.f, inc_pos.z * i);
			m_app_renderer->m_point_light_instance_list.push_back(inst_data);
		}
	}
	point_light_inst_count = static_cast<uint32_t>(m_app_renderer->m_point_light_instance_list.size());


	for (uint32_t i = 0; i < point_light_inst_count; ++i)
	{
		const PointLightInstanceData& light_inst = m_app_renderer->m_point_light_instance_list[i];
		const Light* cur_light = light_inst.light;

		Vector4 light_color(cur_light->GetColor().x, cur_light->GetColor().y, cur_light->GetColor().z, 1.f);

		m_constant_point_light_data[i].LightColor = light_color;
		m_constant_point_light_data[i].LightPosition = Vector4(light_inst.light_position.x,
			light_inst.light_position.y, light_inst.light_position.z, 1.0);
		m_constant_point_light_data[i].LightMiscData.x = cur_light->get_radius();
		m_constant_point_light_data[i].LightMiscData.y = cur_light->GetIntensity();

		Vector2 attenuation_constant = cur_light->get_attenuation_constant();
		m_constant_point_light_data[i].LightMiscData.z = attenuation_constant.x;
		m_constant_point_light_data[i].LightMiscData.w = attenuation_constant.y;
	}

	BufferUpdateDesc update_constant_point_light_buffer_desc = {};
	update_constant_point_light_buffer_desc.m_buffer = m_point_light_buffer;
	update_constant_point_light_buffer_desc.m_pSource = &m_constant_point_light_data;
	update_constant_point_light_buffer_desc.m_size = static_cast<uint32_t>(sizeof(ConstantPointLightData) * point_light_inst_count);

	m_dxrenderer->cmd_update_buffer(update_constant_point_light_buffer_desc);

	m_dxrenderer->cmd_bind_pipeline(m_deferred_shade_pointlight_pipeline);

	Model* sphere_model = m_resourceManager->GetModel(StringId("Sphere.fbx"));

	m_dxrenderer->cmd_bind_vertex_buffer(sphere_model->get_vertex_buffer());
	m_dxrenderer->cmd_bind_index_buffer(sphere_model->get_index_buffer());

	Texture* world_normal_texture = m_deferred_rts[DEFERRED_WORLD_NORMAL]->get_texture();
	Texture* albedo_texture = m_deferred_rts[DEFERRED_ALBEDO]->get_texture();
	Texture* specular_texture = m_deferred_rts[DEFERRED_SPECULAR]->get_texture();
	Texture* depth_texture = m_app_renderer->m_depth_rt->get_texture();

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
	params[4].m_buffers = &m_app_renderer->m_camera_uniform_buffer;

	params[5].m_binding_location = 0;
	params[5].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[5].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[5].m_buffers = &m_point_light_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_deferred_shade_pointlight_pipeline, 6, params);
	m_dxrenderer->cmd_draw_index_instanced(point_light_inst_count, 0, sphere_model->get_index_total_count(), 0, 0);
}