#include "AppRenderer.h"
#include "D3D11_Renderer.h"
#include "Graphics/Models/Model.h"
#include "Material.h"
#include "Light.h"
#include "Managers/CameraManager.h"
#include "Graphics/Camera.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/Camera.h"
#include "Managers/EventManager.h"
#include "Graphics/Deferred/DeferredRenderingInstance.h"
#include "Graphics/DebugRendering/DebugRendering.h"
#include "Events/Camera/CameraRegistrationEvent.h"
#include "Events/Camera/CameraDestructionEvent.h"


AppRenderer::AppRenderer(SDL_Window& sdl_window, ResourceManager* resourceManager,
	CameraManager* cameraManager)
:m_sdl_window(sdl_window),
m_debugRendering(this, resourceManager),
m_resourceManager(resourceManager),
m_cameraManager(cameraManager),
m_deferrredRendering(this, resourceManager),
m_msaa_resolve_pass(this),
m_particleRendering(this),
m_momentShadowMapRendering(this, MomentShadowMapData{2048u, 2048u}),
m_toSkyboxRender(this),
m_iblFilterEnvMapPass(this),
m_brdfLookupTexturePass(this),
m_uiObjectRendering(this),
m_gameTime(0.f)
{
	InitializeRenderer();
}


AppRenderer::~AppRenderer()
{
	Release();
}


void AppRenderer::InnerLoadContent()
{
	m_material_uniform_data_list.reserve(1000);
	m_material_uniform_buffer_list.reserve(1000);

	InitRandomTexture1D();

	DepthStateDesc less_equal_depth_state_desc = {};
	less_equal_depth_state_desc.m_depth_enable = true;
	less_equal_depth_state_desc.m_depth_write_mask = DepthWriteMask::WRITE_DEPTH;
	less_equal_depth_state_desc.m_stencil_enable = false;
	less_equal_depth_state_desc.m_depth_compare_func = CompareFunc::CMP_LEQUAL;

	m_less_equal_depth_state = DXResourceLoader::Create_DepthState(m_dxrenderer, less_equal_depth_state_desc);

	DepthStateDesc testonlyLessEqualDepthStateDesc = {};
	testonlyLessEqualDepthStateDesc.m_depth_enable = true;
	testonlyLessEqualDepthStateDesc.m_depth_write_mask = DepthWriteMask::DONT_WRITE_DEPTH;
	testonlyLessEqualDepthStateDesc.m_stencil_enable = false;
	testonlyLessEqualDepthStateDesc.m_depth_compare_func = CompareFunc::CMP_LEQUAL;

	m_testonlyLessEqualDepthState = DXResourceLoader::Create_DepthState(m_dxrenderer, testonlyLessEqualDepthStateDesc);


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


	RasterizerStateDesc cull_none_state_ms_desc = {};
	cull_none_state_ms_desc.m_cull_mode = CullMode::NONE;
	cull_none_state_ms_desc.m_fill_mode = FillMode::SOLID;
	cull_none_state_ms_desc.m_multiSample = true;

	m_cull_none_rasterizer_ms_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_none_state_ms_desc);

	RasterizerStateDesc cull_front_state_ms_desc = {};
	cull_front_state_ms_desc.m_cull_mode = CullMode::FRONT;
	cull_front_state_ms_desc.m_fill_mode = FillMode::SOLID;
	cull_front_state_ms_desc.m_multiSample = true;

	m_cull_front_rasterizer_ms_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_front_state_ms_desc);

	RasterizerStateDesc cull_back_state_ms_desc = {};
	cull_back_state_ms_desc.m_cull_mode = CullMode::BACK;
	cull_back_state_ms_desc.m_fill_mode = FillMode::SOLID;
	cull_back_state_ms_desc.m_multiSample = true;

	m_cull_back_rasterizer_ms_state = DXResourceLoader::Create_RasterizerState(m_dxrenderer, cull_back_state_ms_desc);


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


	BlendStateDesc additiveBlendingDesc = {};
	additiveBlendingDesc.m_blendOperator[0] = BO_ADD;
	additiveBlendingDesc.m_blendAlphaOperator[0] = BO_ADD;
	additiveBlendingDesc.m_srcFactors[0] = BF_SRC_ALPHA;
	additiveBlendingDesc.m_dstFactors[0] = BF_ONE;
	additiveBlendingDesc.m_srcAlphaFactors[0] = BF_ONE;
	additiveBlendingDesc.m_dstAlphaFactors[0] = BF_ONE;
	additiveBlendingDesc.m_blendStateTarget = BLEND_STATE_RT_0;
	additiveBlendingDesc.m_individualBlend = false;
	additiveBlendingDesc.m_enableAlphaCoverage = false;

	m_additiveBlending = DXResourceLoader::Create_BlendState(m_dxrenderer, additiveBlendingDesc);



	SamplerDesc clamp_linear_sampler_desc = {};
	clamp_linear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE;
	clamp_linear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_linear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_linear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_POINT;
	clamp_linear_sampler_desc.m_max_aniso = 0.f;

	m_clamp_linear_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, clamp_linear_sampler_desc);


	SamplerDesc repeat_linear_sampler_desc = {};
	repeat_linear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	repeat_linear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	repeat_linear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	repeat_linear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_POINT;
	repeat_linear_sampler_desc.m_max_aniso = 0.f;

	m_repeat_linear_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, repeat_linear_sampler_desc);


	SamplerDesc clamp_trillinear_sampler_desc = {};
	clamp_trillinear_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	clamp_trillinear_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_trillinear_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	clamp_trillinear_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_LINEAR;
	clamp_trillinear_sampler_desc.m_max_aniso = 0.f;

	m_trillinear_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, clamp_trillinear_sampler_desc);

	SamplerDesc texture_sampler_desc = {};
	texture_sampler_desc.m_address_u = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	texture_sampler_desc.m_address_v = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	texture_sampler_desc.m_address_w = SamplerAddressMode::ADDRESS_MODE_REPEAT;
	texture_sampler_desc.m_min_filter = SamplerFilterType::FILTER_LINEAR;
	texture_sampler_desc.m_mag_filter = SamplerFilterType::FILTER_LINEAR;
	texture_sampler_desc.m_mipmap_mode = SamplerMipMapMode::MIPMAP_MODE_POINT;
	texture_sampler_desc.m_max_aniso = 8.f;

	m_texture_sampler = DXResourceLoader::Create_Sampler(m_dxrenderer, texture_sampler_desc);

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

	VertexLayout pos3_layout = {};

	pos3_layout.m_atrrib_count = 1;
	pos3_layout.m_attribs[0].m_binding = 0;
	pos3_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos3_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;


	ShaderLoadDesc resolveAppRendererInstancesShaderDesc = {};
	resolveAppRendererInstancesShaderDesc.m_desc.m_vertex_shader_path = "resolve_texture_vert.hlsl";
	resolveAppRendererInstancesShaderDesc.m_desc.m_pixel_shader_path = "resolve_texture_frag.hlsl";
	m_resolveAppRendererInstancesShader = DXResourceLoader::Create_Shader(m_dxrenderer, resolveAppRendererInstancesShaderDesc);

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;

	pipeline_desc.m_graphics_desc = {};

	GraphicsPipelineDesc& resolveAppRendererPipelineDesc = pipeline_desc.m_graphics_desc;
	//resolveAppRendererPipelineDesc.m_blend_state = m_blend_state_one_zero_add;
	resolveAppRendererPipelineDesc.m_depth_state = m_disabled_depth_state;
	resolveAppRendererPipelineDesc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	resolveAppRendererPipelineDesc.m_rasterizer_state = m_cull_none_rasterizer_state;
	resolveAppRendererPipelineDesc.m_shader = m_resolveAppRendererInstancesShader;
	resolveAppRendererPipelineDesc.m_vertex_layout = &pos3_layout;

	m_resolveAppRendererInstancesPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	BufferLoadDesc directional_light_uniform_buffer_desc = {};
	directional_light_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	directional_light_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	directional_light_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	directional_light_uniform_buffer_desc.m_desc.m_debugName = "Directional Light Uniform Buffer";
	directional_light_uniform_buffer_desc.m_rawData = nullptr;
	directional_light_uniform_buffer_desc.m_size = sizeof(DirectionalLightUniformData);

	m_directional_light_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, directional_light_uniform_buffer_desc);


	RenderTarget* swap_chain_rt = m_dxrenderer->GetSwapChain()->m_p_swap_chain_render_target;

	InitializeSkyboxData();
	

}


void AppRenderer::InitRandomTexture1D()
{
	TextureDesc randomTex1DDesc = {};
	randomTex1DDesc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	randomTex1DDesc.m_usageType = Usage_Type::USAGE_IMMUTABLE;
	randomTex1DDesc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	randomTex1DDesc.m_width = 1024;
	randomTex1DDesc.m_mipLevels = 1;
	randomTex1DDesc.m_height = 1;
	randomTex1DDesc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	randomTex1DDesc.m_isSRGB = false;

	Vector4 finalData[1024] = { Vector4(0.0, 0.0, 0.0, 1.0) };
	for (uint32_t i = 0; i < 1024; ++i)
	{
		finalData[i].x = MathUtil::RandF(-1.0f, 1.0f);
		finalData[i].y = MathUtil::RandF(-1.0f, 1.0f);
		finalData[i].z = MathUtil::RandF(-1.0f, 1.0f);
		finalData[i].w = 1.f;
	}
	TextureLoadDesc loadRandomTex1DDesc = {};
	loadRandomTex1DDesc.m_generateMipMap = false;
	loadRandomTex1DDesc.m_rawData = &finalData;
	loadRandomTex1DDesc.m_rawDataOnePixelSize = sizeof(Vector4);
	loadRandomTex1DDesc.m_tex_desc = &randomTex1DDesc;

	m_random1DTexture = DXResourceLoader::Create_Texture(m_dxrenderer, loadRandomTex1DDesc);
}

void AppRenderer::InitializeSkyboxData()
{
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
	//graphics_pipeline_desc.m_depth_state = m_less_equal_depth_state;
	graphics_pipeline_desc.m_depth_state = m_disabled_depth_state;
	graphics_pipeline_desc.m_blend_state = m_skybox_blend_state;
	graphics_pipeline_desc.m_shader = m_skybox_shader;
	graphics_pipeline_desc.m_vertex_layout = &pos4_layout;

	m_skybox_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}



void AppRenderer::InitializeRenderer()
{
	SDL_SysWMinfo sys_info;
	SDL_VERSION(&sys_info.version);
	SDL_GetWindowWMInfo(&m_sdl_window, &sys_info);
	sys_info.info.win.window;

	m_dxrenderer = new DXRenderer(sys_info.info.win.window, true);
	m_dxrenderer->init(1);
}

void AppRenderer::InitializeResources()
{
	InitializeDefaultIBLData(); 
	EventManager::Get()->SubscribeEvent<CameraRegistrationEvent>(this,
		std::bind(&AppRenderer::OnCameraRegistration, this, std::placeholders::_1));

	EventManager::Get()->SubscribeEvent<CameraDestructionEvent>(this,
		std::bind(&AppRenderer::OnCameraDestruction, this, std::placeholders::_1));

	EventManager::Get()->SubscribeEvent<WindowSizeEvent>(this,
		std::bind(&AppRenderer::OnWindowSize, this, std::placeholders::_1));
}


void AppRenderer::InitializeDefaultIBLData()
{
	TextureDesc brdf_lookup_texture_desc = {};
	brdf_lookup_texture_desc.m_arraySize = 1;
	brdf_lookup_texture_desc.m_width = 512;
	brdf_lookup_texture_desc.m_height = 512;
	brdf_lookup_texture_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE | Bind_Flags::BIND_UNORDERED_ACCESS;
	brdf_lookup_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	brdf_lookup_texture_desc.m_mipLevels = 1;
	brdf_lookup_texture_desc.m_imageFormat = DXGI_FORMAT_R16G16_FLOAT;
	brdf_lookup_texture_desc.m_isSRGB = false;
	brdf_lookup_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	brdf_lookup_texture_desc.m_depth = 1;
	brdf_lookup_texture_desc.m_clearVal = ClearValue{ 0.f, 0.f, 0.f, 0.f };

	TextureLoadDesc load_brdf_lookup_texture_desc = {};
	load_brdf_lookup_texture_desc.m_tex_desc = &brdf_lookup_texture_desc;
	load_brdf_lookup_texture_desc.m_file_name = "";

	m_pBRDFLookupTexture = DXResourceLoader::Create_Texture(m_dxrenderer, load_brdf_lookup_texture_desc);

	m_toSkyboxRender.Initialize(m_dxrenderer);
	m_iblFilterEnvMapPass.Initialize(m_dxrenderer);
	m_brdfLookupTexturePass.Initialize(m_dxrenderer);

	m_brdfLookupTexturePass.Render(m_pBRDFLookupTexture);
}

void AppRenderer::LoadContent()
{
	InnerLoadContent();
	m_debugRendering.LoadContent(m_dxrenderer);
	m_deferrredRendering.LoadContent(m_dxrenderer);
	m_msaa_resolve_pass.LoadContent(m_dxrenderer);
	m_particleRendering.LoadContent(m_dxrenderer);
	m_momentShadowMapRendering.LoadContent(m_dxrenderer);
	m_uiObjectRendering.Initialize(m_dxrenderer);
}


void AppRenderer::OnCameraRegistration(const CameraRegistrationEvent* event)
{
	CameraInfo cameraInfo(event->m_camera, event->m_xViewPortPos, event->m_yViewportPos);


	//TODO: use mmeory pool
	AppRendererInstance* appRendererInstance = new AppRendererInstance(this, m_dxrenderer, cameraInfo);
	appRendererInstance->Initialize();
	appRendererInstance->LoadContent();
	m_appRendererInstances[event->m_camera.GetZOrder()] = appRendererInstance;
}

void AppRenderer::OnCameraDestruction(const CameraDestructionEvent* event)
{
	auto findIter = m_appRendererInstances.find(event->m_pCamera->GetZOrder());
	if (findIter == m_appRendererInstances.end())
	{
		return;
	}
	AppRendererInstance* rendererInstance = findIter->second;
	SafeReleaseDelete(rendererInstance);
	m_appRendererInstances.erase(findIter);
}
void AppRenderer::OnWindowSize(const WindowSizeEvent* event)
{

}


void AppRenderer::Release()
{
	EventManager::Get()->UnsubscribeEvent<CameraRegistrationEvent>(this);
	EventManager::Get()->UnsubscribeEvent<CameraDestructionEvent>(this);
	EventManager::Get()->UnsubscribeEvent<WindowSizeEvent>(this);

	SafeReleaseDelete(m_dxrenderer);

	for (Buffer* buffer : m_boneTransformsUniformBufferList)
	{
		SafeReleaseDelete(buffer);
	}
	
	for (auto& pair : m_appRendererInstances)
	{
		SafeReleaseDelete(pair.second);
	}

	for (Buffer* buffer : m_material_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}
	
	m_deferrredRendering.Release();
	m_debugRendering.Release();
	m_momentShadowMapRendering.Release();

	m_toSkyboxRender.Release();
	m_iblFilterEnvMapPass.Release();
	m_brdfLookupTexturePass.Release();
	m_uiObjectRendering.Release();

	SafeReleaseDelete(m_cull_none_rasterizer_state);
	SafeReleaseDelete(m_cull_front_rasterizer_state);
	SafeReleaseDelete(m_cull_back_rasterizer_state);
	SafeReleaseDelete(m_cull_none_wireframe_state);
	SafeReleaseDelete(m_cull_none_rasterizer_ms_state);
	SafeReleaseDelete(m_cull_front_rasterizer_ms_state);
	SafeReleaseDelete(m_cull_back_rasterizer_ms_state);

	SafeReleaseDelete(m_less_equal_depth_state);
	SafeReleaseDelete(m_disabled_depth_state);
	SafeReleaseDelete(m_testonlyLessEqualDepthState);

	SafeReleaseDelete(m_clamp_linear_sampler);
	SafeReleaseDelete(m_repeat_linear_sampler);

	SafeReleaseDelete(m_skybox_pipeline);
	SafeReleaseDelete(m_skybox_shader);
	SafeReleaseDelete(m_skybox_vertices_buffer);

	SafeReleaseDelete(m_pBRDFLookupTexture);

	SafeReleaseDelete(m_trillinear_sampler);
	SafeReleaseDelete(m_texture_sampler);

	SafeReleaseDelete(m_blend_state_one_zero_add);
	SafeReleaseDelete(m_skybox_blend_state);
	SafeReleaseDelete(m_additiveBlending);
	SafeReleaseDelete(m_directional_light_uniform_buffer);
	SafeReleaseDelete(m_random1DTexture);

	SafeReleaseDelete(m_resolveAppRendererInstancesPipeline);
	SafeReleaseDelete(m_resolveAppRendererInstancesShader);
}

void AppRenderer::UpdateBoneTransformsBufferList()
{
	for (uint32_t i = 0; i < m_boneMeshInstancesList.size(); ++i)
	{
		const InstanceRenderData& inst_data = m_boneMeshInstancesList[i].m_instanceRenderData;
		Model* p_ref_model = inst_data.p_ref_model;

		assert(p_ref_model);

		if (i >= m_boneTransformsUniformBufferList.size())
		{
			AddBoneTransformBuffer();
		}

		Buffer* bone_uniform_buffer = m_boneTransformsUniformBufferList[i];
		const BoneTransformationsList& boneTransformationsList = *m_boneMeshInstancesList[i].m_pBoneTransformationsList;

		BufferUpdateDesc update_bone_uniform_desc = {};
		update_bone_uniform_desc.m_buffer = bone_uniform_buffer;
		update_bone_uniform_desc.m_pSource = &boneTransformationsList[0];
		update_bone_uniform_desc.m_size = sizeof(BoneTransformsUniformData);
		m_dxrenderer->cmd_update_buffer(update_bone_uniform_desc);
	}
}

void AppRenderer::AddBoneTransformBuffer()
{
	BufferLoadDesc boneTransformBufferDesc = {};
	boneTransformBufferDesc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	boneTransformBufferDesc.m_desc.m_debugName = "Bone Transforms Uniform Buffer";
	boneTransformBufferDesc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	boneTransformBufferDesc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	boneTransformBufferDesc.m_rawData = nullptr;
	boneTransformBufferDesc.m_size = sizeof(BoneTransformsUniformData);
	Buffer* object_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, boneTransformBufferDesc);
	m_boneTransformsUniformBufferList.push_back(object_uniform_buffer);
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

ParticleRendering& AppRenderer::GetParticleRendering()
{
	return m_particleRendering;
}

void AppRenderer::UpdateAppRenderer(float dt)
{
	m_gameTime += dt;

	DEBUG_CHECKBOX("Draw Mesh", &GraphicsSettings::Draw_Mesh_Flag);
	DEBUG_CHECKBOX("Draw Dynamic AABB Tree", &GraphicsSettings::Draw_Dynamic_AABB_Tree);
	DEBUG_CHECKBOX("Draw Debug Sphere", &GraphicsSettings::Draw_Debug_Mesh_Sphere_Flag);
	DEBUG_CHECKBOX("Draw Contact Points", &PhysicsUtils::Settings::isDrawContactPoints);

	size_t direction_light_num = m_directionLightInstanceList.size();
	if (direction_light_num > 0)
	{
		for (uint32_t i = 0; i < direction_light_num; ++i)
		{
			const DirectionalLightInstanceData& light_inst_data = m_directionLightInstanceList[i];

			m_directional_light_uniform_data.DirectionalLightColor[i] = MathUtil::v3_to_v4(light_inst_data.light->GetColor());
			m_directional_light_uniform_data.DirectionalLightMiscData[i].y = light_inst_data.light->GetIntensity();
			m_directional_light_uniform_data.DirectionalLightMiscData[i].z = light_inst_data.light->GetShadowIntensity();
			m_directional_light_uniform_data.LightDirection[i] = MathUtil::v3_to_v4(light_inst_data.light_direction, 0.0);
		}
	}

	m_directional_light_uniform_data.DirectionalLightUniformMiscData.w = static_cast<float>(direction_light_num);


	m_deferrredRendering.Update(dt);
	m_debugRendering.Update(dt);
	m_particleRendering.Update(dt, m_gameTime);

	for (const auto& pair : m_appRendererInstances)
	{
		pair.second->Update(dt);
	}
}
void AppRenderer::RenderApp()
{
	BakeSkyboxIrradianceData();

	RenderTarget* swap_chain_rt = m_dxrenderer->GetSwapChain()->m_p_swap_chain_render_target;

	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_clear_color_values[0] = swap_chain_rt->get_clear_value();
	load_actions_desc.m_load_actions_color[0] = LoadActionType::CLEAR;

	m_dxrenderer->cmd_bind_render_targets(&swap_chain_rt, 1, nullptr, load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, swap_chain_rt->get_desc().m_texture_desc.m_width,
		swap_chain_rt->get_desc().m_texture_desc.m_height);

	BufferUpdateDesc direction_light_update_desc = {};
	direction_light_update_desc.m_buffer = m_directional_light_uniform_buffer;
	direction_light_update_desc.m_pSource = &m_directional_light_uniform_data;
	direction_light_update_desc.m_size = sizeof(DirectionalLightUniformData);

	m_dxrenderer->cmd_update_buffer(direction_light_update_desc);

	UpdateMaterialUniformBuffer();
	UpdateBoneTransformsBufferList();

	m_deferrredRendering.UpdateUniformBuffer();
	m_debugRendering.UpdateDebugUniformBuffer();

	m_momentShadowMapRendering.RenderShadowMap();

	m_particleRendering.Render();

	for (const auto& pair : m_appRendererInstances)
	{
		pair.second->Render();
	}

	LoadActionsDesc next_load_actions_desc = {};
	next_load_actions_desc.m_clear_color_values[0] = swap_chain_rt->get_clear_value();
	next_load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;

	m_dxrenderer->cmd_bind_render_targets(&swap_chain_rt, 1, nullptr, next_load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, swap_chain_rt->get_desc().m_texture_desc.m_width,
		swap_chain_rt->get_desc().m_texture_desc.m_height);


	ResolveAppRendererInstances();
	m_dxrenderer->execute_queued_cmd();	

	m_uiObjectInstanceRenderDataList.clear();
	m_directionLightInstanceList.clear();
	m_pointLightInstanceList.clear();
	m_basicInstances.clear();
	m_boneMeshInstancesList.clear();
	m_haloEffectInstanceList.clear();
	m_processSkyboxIrradianceInstanceList.clear();
	m_bakedSkyboxIrradianceInstanceList.clear();
	m_debugRendering.ClearInstances();
	m_particleRendering.ClearInstances();
	m_toSkyboxRender.ClearInstancesCount();
}


void AppRenderer::BakeSkyboxIrradianceData()
{
	if (m_processSkyboxIrradianceInstanceList.empty())
	{
		return;
	}

	ProcessSkyboxIrradianceInstanceData& processSkyboxIrradianceData = m_processSkyboxIrradianceInstanceList[0];

	m_toSkyboxRender.Render(processSkyboxIrradianceData.m_pSkyboxEquirectangularTexture,
		processSkyboxIrradianceData.m_bakedData.m_pSkyboxTexture);

	m_toSkyboxRender.Render(processSkyboxIrradianceData.m_pSkyboxEquirectangularIrradianceTexture,
		processSkyboxIrradianceData.m_bakedData.m_pSkyboxIrradianceTexture);

	m_iblFilterEnvMapPass.Render(processSkyboxIrradianceData.m_bakedData.m_pSkyboxTexture, processSkyboxIrradianceData.m_bakedData.m_pIBLPrefilteredEnvMapTexture);

	*processSkyboxIrradianceData.m_hasBaked = true;

	m_processSkyboxIrradianceInstanceList.erase(m_processSkyboxIrradianceInstanceList.begin());
}

void AppRenderer::ResolveAppRendererInstances()
{
	m_dxrenderer->cmd_bind_pipeline(m_resolveAppRendererInstancesPipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(nullptr);


	DescriptorData params[2] = {};
	for (auto& pair : m_appRendererInstances)
	{
		Texture* rendererTexture = pair.second->m_finalOutputRT->get_texture();

		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
		params[0].m_shader_stages = Shader_Stages::PIXEL_STAGE;
		params[0].m_textures = &rendererTexture;

		params[1].m_binding_location = 0;
		params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[1].m_buffers = &pair.second->m_resolveUniformBuffer;
		params[1].m_shader_stages = Shader_Stages::VERTEX_STAGE;

		m_dxrenderer->cmd_bind_descriptor(m_resolveAppRendererInstancesPipeline, 2, params);
		m_dxrenderer->cmd_draw(6, 0);
	}
}

void AppRenderer::UpdateMaterialUniformBuffer()
{
	if (m_basicInstances.empty())
	{
		return;
	}
	
	struct MaterialFullData
	{
		Material* material;
		const InstanceRenderData* instanceRenderData;
	};


	std::vector<MaterialFullData> finalMaterialList;
	std::vector<const InstanceRenderData*> allInstancesList;

	for (uint64_t i = 0; i < m_basicInstances.size(); ++i)
	{
		const InstanceRenderData& inst_data = m_basicInstances[i];
		allInstancesList.push_back(&inst_data);
	}

	for (uint64_t i = 0; i < m_boneMeshInstancesList.size(); ++i)
	{
		const InstanceRenderData& inst_data = m_boneMeshInstancesList[i].m_instanceRenderData;
		allInstancesList.push_back(&inst_data);
	}

	for (uint64_t i = 0; i < allInstancesList.size(); ++i)
	{
		const InstanceRenderData& inst_data = *allInstancesList[i];

		Model* p_ref_model = inst_data.p_ref_model;
		Material* p_ref_material = inst_data.p_ref_material;

		assert(p_ref_material && p_ref_model);

		const Model::MeshesList& meshes_list = p_ref_model->GetMeshesList();
		uint32_t mesh_instance_count = std::max(1u, static_cast<unsigned int>(meshes_list.size()));

		for (uint32_t mesh_index = 0; mesh_index < mesh_instance_count; ++mesh_index)
		{

			Material* cur_material_instance = meshes_list.size() <= 1 ? p_ref_material : meshes_list[mesh_index].get_material();
			//sometimes despite having multiple child of mesh instance it may still not have its own material
			if (!cur_material_instance)
			{
				cur_material_instance = p_ref_material;
			}
			finalMaterialList.push_back(MaterialFullData{ cur_material_instance, &inst_data });
		}
	}


	for (uint64_t material_index = 0; material_index < finalMaterialList.size(); ++material_index)
	{
		MaterialFullData& materialFullData = finalMaterialList[material_index];

		Material* curMaterialInstance = materialFullData.material;
		const InstanceRenderData* instanceRenderData = materialFullData.instanceRenderData;

		if (material_index >= m_material_uniform_buffer_list.size())
		{
			AddMaterialUniformBuffer();
		}

		Buffer* material_uniform_buffer = m_material_uniform_buffer_list[material_index];
		m_material_uniform_data_list[material_index] = {};
		m_material_uniform_data_list[material_index].DiffuseColor = curMaterialInstance->GetDiffuseColor();
		m_material_uniform_data_list[material_index].SpecularColor = curMaterialInstance->GetSpecularColor();
		m_material_uniform_data_list[material_index].MaterialMiscData.w = (float)curMaterialInstance->GetShaderMaterialType();
		m_material_uniform_data_list[material_index].MaterialMiscData.x = instanceRenderData->uv_tiling.x;
		m_material_uniform_data_list[material_index].MaterialMiscData.y = instanceRenderData->uv_tiling.y;
		m_material_uniform_data_list[material_index].MaterialMiscData2.x = curMaterialInstance->GetRoughnessValue();
		m_material_uniform_data_list[material_index].MaterialMiscData2.y = curMaterialInstance->GetMetallicValue();

		BufferUpdateDesc update_material_uniform_desc = {};
		update_material_uniform_desc.m_buffer = m_material_uniform_buffer_list[material_index];
		update_material_uniform_desc.m_pSource = &m_material_uniform_data_list[material_index];
		update_material_uniform_desc.m_size = sizeof(MaterialUniformData);
		m_dxrenderer->cmd_update_buffer(update_material_uniform_desc);

	}
}

void AppRenderer::RegisterBasicInstance(const InstanceRenderData& instanceRenderData)
{
	if (GraphicsSettings::Draw_Mesh_Flag)
	{
		m_basicInstances.push_back(instanceRenderData);
	}
}

void AppRenderer::RegisterDirectionalLightInstance(const DirectionalLightInstanceData& directionalLightInstanceData)
{
	m_directionLightInstanceList.push_back(directionalLightInstanceData);
}

void AppRenderer::RegisterHaloEffectInstance(const HaloEffectInstanceData& haloEffectData)
{
	m_haloEffectInstanceList.push_back(haloEffectData);
}

void AppRenderer::RegisterPointLightInstance(const PointLightInstanceData& pointLightInstanceData)
{
	m_pointLightInstanceList.push_back(pointLightInstanceData);
}

void AppRenderer::RegisterBoneMeshInstance(const BoneMeshInstanceRenderData& boneMeshInstanceData)
{
	m_boneMeshInstancesList.push_back(boneMeshInstanceData);
}

void AppRenderer::RegisterUIObjectInstance(const UIObjectInstanceRenderData& uiObjectInstanceData)
{
	m_uiObjectInstanceRenderDataList.push_back(uiObjectInstanceData);
}

void AppRenderer::RegisterProcessSkyboxIrradianceInstance(const ProcessSkyboxIrradianceInstanceData& processInstanceData)
{
	m_processSkyboxIrradianceInstanceList.push_back(processInstanceData);
}

void AppRenderer::RegisterBakedSkyboxIrradianceInstance(const BakedSkyboxIrradianceInstanceData& bakedInstanceData)
{
	m_bakedSkyboxIrradianceInstanceList.push_back(bakedInstanceData);
}

void AppRenderer::AddObjectUniformBuffer(BufferList& objectUniformBufferList,
	std::vector<ObjectUniformData>& objectUniformDataList)
{
	BufferLoadDesc object_uniform_buffer_desc = {};
	object_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	object_uniform_buffer_desc.m_desc.m_debugName = "Object Uniform Buffer";
	object_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	object_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	object_uniform_buffer_desc.m_rawData = nullptr;
	object_uniform_buffer_desc.m_size = sizeof(ObjectUniformData);

	Buffer* object_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, object_uniform_buffer_desc);
	objectUniformBufferList.push_back(object_uniform_buffer);
	objectUniformDataList.push_back(ObjectUniformData());
}

void AppRenderer::AddMaterialUniformBuffer()
{
	BufferLoadDesc material_uniform_buffer_desc = {};
	material_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	material_uniform_buffer_desc.m_desc.m_debugName = "Material Uniform Buffer";
	material_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	material_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	material_uniform_buffer_desc.m_rawData = nullptr;
	material_uniform_buffer_desc.m_size = sizeof(MaterialUniformData);

	Buffer* material_uniform_buffer = DXResourceLoader::Create_Buffer(
		m_dxrenderer, material_uniform_buffer_desc);

	m_material_uniform_buffer_list.push_back(material_uniform_buffer);
	m_material_uniform_data_list.push_back(MaterialUniformData());

}




void AppRenderer::PresentApp()
{
	m_dxrenderer->present_swap_chain();
}
