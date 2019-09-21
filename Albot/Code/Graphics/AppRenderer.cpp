#include "AppRenderer.h"
#include "D3D11_Renderer.h"
#include "Model.h"
#include "Material.h"
#include "Light.h"
#include "Managers/CameraManager.h"
#include "Graphics/Camera.h"


extern CameraManager* gCameraManager;

AppRenderer::AppRenderer(SDL_Window& sdl_window, ResourceManager* resourceManager,
	CameraManager* cameraManager)
:m_sdl_window(sdl_window),
m_debugRendering(this, resourceManager),
m_resourceManager(resourceManager),
m_cameraManager(cameraManager)
{
	//TODO: call initialize manually
	Initialize();
}


AppRenderer::~AppRenderer()
{
}


void AppRenderer::LoadContent()
{
	DepthStateDesc less_equal_depth_state_desc = {};
	less_equal_depth_state_desc.m_depth_enable = true;
	less_equal_depth_state_desc.m_depth_write_mask = DepthWriteMask::WRITE_DEPTH;
	less_equal_depth_state_desc.m_stencil_enable = false;
	less_equal_depth_state_desc.m_depth_compare_func = CompareFunc::CMP_LEQUAL;

	m_less_equal_depth_state = DXResourceLoader::Create_DepthState(m_dxrenderer, less_equal_depth_state_desc);


	DepthStateDesc disabled_depth_state_desc = {};
	disabled_depth_state_desc.m_depth_enable = false;
	disabled_depth_state_desc.m_depth_write_mask = DepthWriteMask::DONT_WRITE_DEPTH;
	disabled_depth_state_desc.m_stencil_enable = false;
	disabled_depth_state_desc.m_depth_compare_func = CompareFunc::CMP_NEVER;

	m_disabled_depth_state = DXResourceLoader::Create_DepthState(m_dxrenderer, disabled_depth_state_desc);

	

	RasterizerStateDesc cull_none_state_desc = {};
	cull_none_state_desc.m_cull_mode = CullMode::NONE;
	cull_none_state_desc.m_fill_mode = FillMode::SOLID;

	m_cull_none_rasterizer_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_none_state_desc);

	RasterizerStateDesc cull_front_state_desc = {};
	cull_front_state_desc.m_cull_mode = CullMode::FRONT;
	cull_front_state_desc.m_fill_mode = FillMode::SOLID;

	m_cull_front_rasterizer_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_front_state_desc);

	RasterizerStateDesc cull_back_state_desc = {};
	cull_back_state_desc.m_cull_mode = CullMode::BACK;
	cull_back_state_desc.m_fill_mode = FillMode::SOLID;

	m_cull_back_rasterizer_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_back_state_desc);


	RasterizerStateDesc cull_none_wireframe_state_desc = {};
	cull_none_wireframe_state_desc.m_cull_mode = CullMode::NONE;
	cull_none_wireframe_state_desc.m_fill_mode = FillMode::WIREFRAME;

	m_cull_none_wireframe_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_none_wireframe_state_desc);


	BlendStateDesc one_zero_add_blend_state_desc = {};
	one_zero_add_blend_state_desc.m_srcFactors[0] = BlendFactor::BF_ONE;
	one_zero_add_blend_state_desc.m_dstFactors[0] = BlendFactor::BF_ONE;
	one_zero_add_blend_state_desc.m_srcAlphaFactors[0] = BlendFactor::BF_ZERO;
	one_zero_add_blend_state_desc.m_dstAlphaFactors[0] = BlendFactor::BF_ONE;
	one_zero_add_blend_state_desc.m_blendOperator[0] = BlendOperator::BO_ADD;
	one_zero_add_blend_state_desc.m_blendAlphaOperator[0] = BlendOperator::BO_ADD;
	one_zero_add_blend_state_desc.m_blendStateTarget = BlendStateTargets::BLEND_STATE_RT_0;
	one_zero_add_blend_state_desc.m_individualBlend = false;

	m_blend_state_one_zero_add = DXResourceLoader::Create_BlendState(m_dxrenderer, one_zero_add_blend_state_desc);


	BlendStateDesc blend_state_skybox_desc = {};
	blend_state_skybox_desc.m_blendOperator[0] = BO_ADD;
	blend_state_skybox_desc.m_blendAlphaOperator[0] = BO_ADD;

	blend_state_skybox_desc.m_srcFactors[0] = BF_ONE_MINUS_DST_ALPHA;
	blend_state_skybox_desc.m_dstFactors[0] = BF_DST_ALPHA;

	blend_state_skybox_desc.m_srcAlphaFactors[0] = BF_ZERO;
	blend_state_skybox_desc.m_dstAlphaFactors[0] = BF_ONE;

	//blend_state_skybox_desc.mMasks[0] = ALL;
	blend_state_skybox_desc.m_blendStateTarget = BLEND_STATE_RT_0;
	blend_state_skybox_desc.m_individualBlend = false;
	m_skybox_blend_state = DXResourceLoader::Create_BlendState(m_dxrenderer, blend_state_skybox_desc);


	SamplerDesc clamp_linear_sampler_desc = {};
	clamp_linear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_linear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_linear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_POINT;
	clamp_linear_sampler_desc.m_max_aniso = 0.f;

	m_clamp_linear_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, clamp_linear_sampler_desc);


	SamplerDesc clamp_trillinear_sampler_desc = {};
	clamp_trillinear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_trillinear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_trillinear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_LINEAR;
	clamp_trillinear_sampler_desc.m_max_aniso = 0.f;

	m_trillinear_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, clamp_trillinear_sampler_desc);

	SamplerDesc repeat_linear_sampler_desc = {};
	repeat_linear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	repeat_linear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	repeat_linear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_POINT;
	repeat_linear_sampler_desc.m_max_aniso = 8.f;

	m_texture_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, repeat_linear_sampler_desc);

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

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;

	pipeline_desc.m_graphics_desc = {};


	pipeline_desc.m_graphics_desc = {};

	

	BufferLoadDesc camera_uniform_buffer_desc = {};
	camera_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	camera_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	camera_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	camera_uniform_buffer_desc.m_desc.m_debugName = "Camera Uniform Buffer";
	camera_uniform_buffer_desc.m_rawData = nullptr;
	camera_uniform_buffer_desc.m_size = sizeof(CameraUniformData);

	m_camera_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, camera_uniform_buffer_desc);


	BufferLoadDesc point_light_uniform_buffer_desc = {};
	point_light_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	point_light_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	point_light_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	point_light_uniform_buffer_desc.m_desc.m_debugName = "Point Light Uniform Buffer";
	point_light_uniform_buffer_desc.m_rawData = nullptr;
	point_light_uniform_buffer_desc.m_size = sizeof(PointLightUniformData);

	m_point_light_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, point_light_uniform_buffer_desc);


	BufferLoadDesc directional_light_uniform_buffer_desc = {};
	directional_light_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	directional_light_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	directional_light_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	directional_light_uniform_buffer_desc.m_desc.m_debugName = "Directional Light Uniform Buffer";
	directional_light_uniform_buffer_desc.m_rawData = nullptr;
	directional_light_uniform_buffer_desc.m_size = sizeof(DirectionalLightUniformData);

	m_directional_light_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, directional_light_uniform_buffer_desc);


	RenderTarget* swap_chain_rt = m_dxrenderer->get_swap_chain()->m_p_swap_chain_render_target;

	RenderTargetDesc depth_rt_desc = {};
	depth_rt_desc.m_texture_desc.m_bindFlags = Bind_Flags::BIND_DEPTH_STENCIL | BIND_SHADER_RESOURCE;
	depth_rt_desc.m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	depth_rt_desc.m_texture_desc.m_width = swap_chain_rt->get_desc().m_texture_desc.m_width;
	depth_rt_desc.m_texture_desc.m_height = swap_chain_rt->get_desc().m_texture_desc.m_height;
	depth_rt_desc.m_texture_desc.m_clear_val = ClearValue{ 1.0f, 0.0 };
	depth_rt_desc.m_texture_desc.m_mip_levels = 1;
	depth_rt_desc.m_texture_desc.m_image_format = DXGI_FORMAT_D32_FLOAT;
	depth_rt_desc.m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	depth_rt_desc.m_texture_desc.m_depth = 1;

	m_depth_rt = DXResourceLoader::Create_RenderTarget(m_dxrenderer, depth_rt_desc);

	load_skybox_content();
	

}

void AppRenderer::load_skybox_content()
{
	
	BufferLoadDesc skybox_uniform_buffer_desc = {};
	skybox_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	skybox_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	skybox_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	skybox_uniform_buffer_desc.m_desc.m_debugName = "Skybox camera uniform data";
	skybox_uniform_buffer_desc.m_size = sizeof(SkyboxUniformData);
	skybox_uniform_buffer_desc.m_rawData = nullptr;

	m_skybox_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, skybox_uniform_buffer_desc);


	float skyBoxPoints[] = 
	{
			0.5f,  -0.5f, -0.5f, 1.0f,    // -z
			-0.5f, -0.5f, -0.5f, 1.0f,  -0.5f, 0.5f,  -0.5f, 1.0f,  -0.5f, 0.5f,
			-0.5f, 1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,

			-0.5f, -0.5f, 0.5f,  1.0f,    //-x
			-0.5f, -0.5f, -0.5f, 1.0f,  -0.5f, 0.5f,  -0.5f, 1.0f,  -0.5f, 0.5f,
			-0.5f, 1.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  -0.5f, -0.5f, 0.5f,  1.0f,

			0.5f,  -0.5f, -0.5f, 1.0f,    //+x
			0.5f,  -0.5f, 0.5f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.5f,  0.5f,
			0.5f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,

			-0.5f, -0.5f, 0.5f,  1.0f,    // +z
			-0.5f, 0.5f,  0.5f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.5f,  0.5f,
			0.5f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  -0.5f, -0.5f, 0.5f,  1.0f,

			-0.5f, 0.5f,  -0.5f, 1.0f,    //+y
			0.5f,  0.5f,  -0.5f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.5f,  0.5f,
			0.5f,  1.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  -0.5f, 0.5f,  -0.5f, 1.0f,

			0.5f,  -0.5f, 0.5f,  1.0f,    //-y
			0.5f,  -0.5f, -0.5f, 1.0f,  -0.5f, -0.5f, -0.5f, 1.0f,  -0.5f, -0.5f,
			-0.5f, 1.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,
	};


	BufferLoadDesc skybox_vertices_buffer_desc = {};
	skybox_vertices_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
	skybox_vertices_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	skybox_vertices_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_IMMUTABLE;
	skybox_vertices_buffer_desc.m_desc.m_debugName = "Skybox Vertex Buffer";
	//skybox_vertices_buffer_desc.m_desc.m_vertex_stride_size = sizeof(float) * 3;
	skybox_vertices_buffer_desc.m_desc.m_vertexStride = sizeof(float) * 4;
	skybox_vertices_buffer_desc.m_size = (skybox_vertices_buffer_desc.m_desc.m_vertexStride * 6 * 6);
	//skybox_vertices_buffer_desc.m_rawData = &skybox_vertices[0];
	skybox_vertices_buffer_desc.m_rawData = skyBoxPoints;

	m_skybox_vertices_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, 
		skybox_vertices_buffer_desc);


	ShaderLoadDesc skybox_shader_desc = {};
	skybox_shader_desc.m_desc.m_vertex_shader_path = "skybox_vert.hlsl";
	skybox_shader_desc.m_desc.m_pixel_shader_path = "skybox_frag.hlsl";
	
	m_skybox_shader = DXResourceLoader::Create_Shader(m_dxrenderer, skybox_shader_desc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;

	VertexLayout pos4_layout = {};

	pos4_layout.m_atrrib_count = 1;
	pos4_layout.m_attribs[0].m_binding = 0;
	//pos3_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos4_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pos4_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;


	GraphicsPipelineDesc& graphics_pipeline_desc = pipeline_desc.m_graphics_desc;
	graphics_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	graphics_pipeline_desc.m_render_target_count = 1;
	graphics_pipeline_desc.m_rasterizer_state = m_cull_none_rasterizer_state;
	graphics_pipeline_desc.m_depth_state = m_less_equal_depth_state;
	//graphics_pipeline_desc.m_depth_state = g_disabled_depth_state;
	//graphics_pipeline_desc.m_blend_state = m_skybox_blend_state;
	graphics_pipeline_desc.m_shader = m_skybox_shader;
	graphics_pipeline_desc.m_vertex_layout = &pos4_layout;

	m_skybox_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}



void AppRenderer::Initialize()
{
	SDL_SysWMinfo sys_info;
	SDL_VERSION(&sys_info.version);
	SDL_GetWindowWMInfo(&m_sdl_window, &sys_info);
	sys_info.info.win.window;

	m_dxrenderer = new DXRenderer(sys_info.info.win.window, true);
	m_dxrenderer->init();
	LoadContent();
	m_debugRendering.LoadContent(m_dxrenderer);
}


void AppRenderer::Release()
{
	SafeReleaseDelete(m_dxrenderer);

	for (Buffer* buffer : m_object_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}

	for (Buffer* buffer : m_material_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}
	
	
	m_debugRendering.Release();

	SafeReleaseDelete(m_cull_none_rasterizer_state);
	SafeReleaseDelete(m_cull_front_rasterizer_state);
	SafeReleaseDelete(m_cull_back_rasterizer_state);
	SafeReleaseDelete(m_cull_none_wireframe_state);

	SafeReleaseDelete(m_less_equal_depth_state);
	SafeReleaseDelete(m_disabled_depth_state);

	SafeReleaseDelete(m_depth_rt);
	SafeReleaseDelete(m_clamp_linear_sampler);



	SafeReleaseDelete(m_basic_pipeline);
	SafeReleaseDelete(m_basic_shader);

	SafeReleaseDelete(m_skybox_pipeline);
	SafeReleaseDelete(m_skybox_shader);
	SafeReleaseDelete(m_skybox_vertices_buffer);
	SafeReleaseDelete(m_skybox_texture);

	SafeReleaseDelete(m_trillinear_sampler);
	SafeReleaseDelete(m_texture_sampler);

	SafeReleaseDelete(m_blend_state_one_zero_add);
	SafeReleaseDelete(m_skybox_blend_state);


	SafeReleaseDelete(m_directional_light_uniform_buffer);
	SafeReleaseDelete(m_point_light_uniform_buffer);
	SafeReleaseDelete(m_camera_uniform_buffer);
	SafeReleaseDelete(m_skybox_uniform_buffer);
}

DXRenderer* AppRenderer::GetDXRenderer()
{
	return m_dxrenderer;
}

SDL_Window& AppRenderer::GetSDLWindow()
{
	return m_sdl_window;
}


DebugRendering& AppRenderer::GetDebugRendering()
{
	return m_debugRendering;
}

void AppRenderer::UpdateAppRenderer(float dt)
{
	//TODO: update camera matrices
	CameraManager* cameraManager = m_cameraManager;

	const CameraInfo* cameraInfo = cameraManager->GetCameraInfo("Main");
	Camera* main_camera = cameraInfo->m_camera;

	RenderTarget* swap_chain_rt = m_dxrenderer->get_swap_chain()->m_p_swap_chain_render_target;

	main_camera->update_view_matrix();
	main_camera->update_projection_matrix();
	main_camera->update_view_projection_matrix();
	
	Vector3 camera_pos = main_camera->GetCameraPosition();
	m_camera_uniform_data.CameraPosition = MathUtil::v3_to_v4(camera_pos, 1.f);
	m_camera_uniform_data.ViewMat = main_camera->GetViewMatrix();
	m_camera_uniform_data.ProjectionMat = main_camera->GetProjectionMatrix();
	m_camera_uniform_data.InvProjectionMat = main_camera->GetInvProjectionMatrix();
	m_camera_uniform_data.InvViewMat = main_camera->GetInvViewMatrix();
	m_camera_uniform_data.ViewProjectionMat = main_camera->GetViewProjectionMatrix();
	m_camera_uniform_data.InvViewProjectionMat = main_camera->GetInvViewProjectionMatrix();
	m_camera_uniform_data.WindowSize = Vector2((float)swap_chain_rt->get_desc().m_texture_desc.m_width,
		(float)swap_chain_rt->get_desc().m_texture_desc.m_height);

	Matrix no_position_view_mat = m_camera_uniform_data.ViewMat;
	no_position_view_mat._41 = 0.f;
	no_position_view_mat._42 = 0.f;
	no_position_view_mat._43 = 0.f;

	m_skybox_uniform_data.ModelViewProjectionMat =
		no_position_view_mat * m_camera_uniform_data.ProjectionMat;
}

void AppRenderer::RenderApp()
{
	RenderTarget* swap_chain_rt = m_dxrenderer->get_swap_chain()->m_p_swap_chain_render_target;

	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_clear_color_values[0] = swap_chain_rt->get_clear_value();
	load_actions_desc.m_load_actions_color[0] = LoadActionType::CLEAR;
	load_actions_desc.m_clear_depth_stencil = m_depth_rt->get_clear_value();
	load_actions_desc.m_load_action_depth = LoadActionType::CLEAR;

	m_dxrenderer->cmd_bind_render_targets(&swap_chain_rt, 1, m_depth_rt, load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, swap_chain_rt->get_desc().m_texture_desc.m_width,
		swap_chain_rt->get_desc().m_texture_desc.m_height);


	BufferUpdateDesc update_camera_desc = {};
	update_camera_desc.m_buffer = m_camera_uniform_buffer;
	update_camera_desc.m_pSource = &m_camera_uniform_data;
	update_camera_desc.m_size = sizeof(CameraUniformData);

	m_dxrenderer->cmd_update_buffer(update_camera_desc);

	m_debugRendering.render_debug_scene();

	m_dxrenderer->execute_queued_cmd();	
}

void AppRenderer::render_skybox()
{
	BufferUpdateDesc update_skybox_buffer_desc = {};
	update_skybox_buffer_desc.m_buffer = m_skybox_uniform_buffer;
	update_skybox_buffer_desc.m_pSource = &m_skybox_uniform_data;
	update_skybox_buffer_desc.m_size = sizeof(SkyboxUniformData);
	m_dxrenderer->cmd_update_buffer(update_skybox_buffer_desc);

	m_dxrenderer->cmd_bind_pipeline(m_skybox_pipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(m_skybox_vertices_buffer);

	DescriptorData params[3] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_buffers = &m_skybox_uniform_buffer;

	params[1].m_binding_location = 0;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
	params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[1].m_textures = &m_skybox_texture;

	params[2].m_binding_location = 0;
	params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
	params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
	params[2].m_samplers = &m_trillinear_sampler;

	m_dxrenderer->cmd_bind_descriptor(m_skybox_pipeline, 3, params);
	m_dxrenderer->cmd_draw(36, 0);
}

void AppRenderer::PresentApp()
{
	m_dxrenderer->present_swap_chain();
}
