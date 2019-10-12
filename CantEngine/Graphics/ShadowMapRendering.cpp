#include "stdafx.h"
#include "ShadowMapRendering.h"
#include "DepthPassRendering.h"

ShadowMapRendering::ShadowMapRendering(AppRenderer* app_renderer, const ShadowMapData& shadow_map_data)
	:m_appRenderer(app_renderer),
	m_depth_pass_rendering(nullptr),
	m_shadow_map_data(shadow_map_data)
{
}


ShadowMapRendering::~ShadowMapRendering()
{
}


void ShadowMapRendering::Release()
{
	SafeReleaseDelete(m_depth_pass_rendering);
	SafeReleaseDelete(m_shadow_map_rt);
	SafeReleaseDelete(m_light_camera_uniform_buffer);
}

RenderTarget*& ShadowMapRendering::get_shadow_map_rt()
{
	return m_shadow_map_rt;
}


void ShadowMapRendering::load_content(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;
	//TODO: probably shadow MSAA is possible we will see
	m_depth_pass_rendering = new DepthPassRendering(m_appRenderer, 1);
	m_depth_pass_rendering->load_content(dxrenderer);


	const SwapChain& swap_chain = m_dxrenderer->GetRefSwapChain();

	RenderTargetDesc shadow_map_rt_desc = {};
	shadow_map_rt_desc.m_texture_desc.m_bindFlags = Bind_Flags::BIND_DEPTH_STENCIL | Bind_Flags::BIND_SHADER_RESOURCE;
	shadow_map_rt_desc.m_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	shadow_map_rt_desc.m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	shadow_map_rt_desc.m_texture_desc.m_clearVal = ClearValue{ 1.f, 0.f };
	shadow_map_rt_desc.m_texture_desc.m_imageFormat = DXGI_FORMAT_D32_FLOAT;
	shadow_map_rt_desc.m_texture_desc.m_depth = 1;
	shadow_map_rt_desc.m_texture_desc.m_mipLevels = 1;
	shadow_map_rt_desc.m_texture_desc.m_sampleCount = SAMPLE_COUNT_1;
	shadow_map_rt_desc.m_texture_desc.m_width = m_shadow_map_data.m_map_width;
	shadow_map_rt_desc.m_texture_desc.m_height = m_shadow_map_data.m_map_height;
	shadow_map_rt_desc.m_texture_desc.m_is_srgb = false;

	m_shadow_map_rt = DXResourceLoader::Create_RenderTarget(dxrenderer, shadow_map_rt_desc);

	BufferLoadDesc light_camera_uniform_buffer_desc = {};
	light_camera_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	light_camera_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	light_camera_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	light_camera_uniform_buffer_desc.m_desc.m_debugName = "Light Camera Buffer";
	light_camera_uniform_buffer_desc.m_size = sizeof(CameraUniformData);
	light_camera_uniform_buffer_desc.m_rawData = nullptr;

	m_light_camera_uniform_buffer = DXResourceLoader::Create_Buffer(dxrenderer, light_camera_uniform_buffer_desc);

}

void ShadowMapRendering::render_shadow_map()
{
	if (m_appRenderer->m_directionLightInstanceList.empty())
	{
		return;
	}

	DirectionalLightInstanceData& directional_light_inst_data = m_appRenderer->m_directionLightInstanceList[0];

	Vector3 light_view_pos = (directional_light_inst_data.light_direction * -300.f);
	Vector3 right_dir = directional_light_inst_data.light_direction.Cross(Vector3(0.f, 1.f, 0.f));
	Vector3 new_up_dir =  right_dir.Cross(directional_light_inst_data.light_direction);
	new_up_dir.Normalize();

	m_shadow_camera_uniform_data.ViewMat = Matrix::CreateLookAt(light_view_pos, 
		light_view_pos + directional_light_inst_data.light_direction, new_up_dir);

	static const float ortho_width = 120.f;
	static const float ortho_height = 120.f;

	Matrix shadow_orthographic_matrix = Matrix::CreateOrthographic(ortho_width, ortho_height, 0.1f, 400.f);

	m_shadow_camera_uniform_data.ProjectionMat = shadow_orthographic_matrix;
	m_shadow_camera_uniform_data.ViewProjectionMat = m_shadow_camera_uniform_data.ViewMat * m_shadow_camera_uniform_data.ProjectionMat;
	m_shadow_camera_uniform_data.CameraViewportSize.x = (float)m_shadow_map_data.m_map_width;
	m_shadow_camera_uniform_data.CameraViewportSize.y = (float)m_shadow_map_data.m_map_height;

	BufferUpdateDesc light_camera_buffer_update_desc = {};
	light_camera_buffer_update_desc.m_buffer = m_light_camera_uniform_buffer;
	light_camera_buffer_update_desc.m_pSource = &m_shadow_camera_uniform_data;
	light_camera_buffer_update_desc.m_size = sizeof(CameraUniformData);
	
	m_dxrenderer->cmd_update_buffer(light_camera_buffer_update_desc);

	DepthPassContext depth_pass_context(m_shadow_map_rt, 
		&m_shadow_camera_uniform_data, &m_appRenderer->m_basicInstances);

	m_depth_pass_rendering->render_depth_pass(depth_pass_context);

}