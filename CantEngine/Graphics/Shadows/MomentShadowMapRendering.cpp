#include "stdafx.h"
#include "MomentShadowMapRendering.h"
#include "Graphics/DepthPassRendering.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/GraphicsSettings.h"

MomentShadowMapRendering::MomentShadowMapRendering(AppRenderer* app_renderer, const MomentShadowMapData& shadow_map_data)
	:m_appRenderer(app_renderer),
	m_depth_pass_rendering(nullptr),
	m_moment_shadow_map_data(shadow_map_data),
	m_blur_image_pass(app_renderer, BLUR_CHANNEL_FOUR, 5),
	m_focusPoint(0.f)
{
}


MomentShadowMapRendering::~MomentShadowMapRendering()
{
}


void MomentShadowMapRendering::Release()
{
	SafeReleaseDelete(m_depth_pass_rendering);
	SafeReleaseDelete(m_moment_shadow_map_rt);
	SafeReleaseDelete(m_depth_rt);
	SafeReleaseDelete(m_light_camera_uniform_buffer);
	SafeReleasePoolAllocatorFree(m_intermediate_texture);
	SafeReleasePoolAllocatorFree(m_output_texture);
	m_blur_image_pass.Release();
}

RenderTarget*& MomentShadowMapRendering::get_shadow_map_rt()
{
	return m_moment_shadow_map_rt;
}

void MomentShadowMapRendering::SetFocusPoint(const Vector3& focusPoint)
{
	m_focusPoint = focusPoint;
}

void MomentShadowMapRendering::LoadContent(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;
	//TODO: probably shadow MSAA is possible we will see
	m_depth_pass_rendering = new DepthPassRendering(m_appRenderer, 1, DepthPassContextType::FOUR_MOMENT_Z_BUFFER);
	m_depth_pass_rendering->LoadContent(dxrenderer);

	RenderTargetDesc depth_map_rt_desc = {};
	depth_map_rt_desc.m_texture_desc.m_bindFlags = Bind_Flags::BIND_DEPTH_STENCIL;
	depth_map_rt_desc.m_texture_desc. m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	depth_map_rt_desc.m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	depth_map_rt_desc.m_texture_desc.m_clearVal = ClearValue{ 1.f, 0.f };
	depth_map_rt_desc.m_texture_desc.m_imageFormat = DXGI_FORMAT_D32_FLOAT;
	depth_map_rt_desc.m_texture_desc.m_depth = 1;
	depth_map_rt_desc.m_texture_desc.m_mipLevels = 1;
	depth_map_rt_desc.m_texture_desc.m_sampleCount = SAMPLE_COUNT_1;
	depth_map_rt_desc.m_texture_desc.m_width = m_moment_shadow_map_data.m_map_width;
	depth_map_rt_desc.m_texture_desc.m_height = m_moment_shadow_map_data.m_map_height;
	depth_map_rt_desc.m_texture_desc.m_isSRGB = false;

	m_depth_rt = DXResourceLoader::Create_RenderTarget(dxrenderer, depth_map_rt_desc);


	RenderTargetDesc shadow_map_rt_desc = {};
	shadow_map_rt_desc.m_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | Bind_Flags::BIND_SHADER_RESOURCE;
	shadow_map_rt_desc.m_texture_desc. m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	shadow_map_rt_desc.m_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	shadow_map_rt_desc.m_texture_desc.m_clearVal = ClearValue{ 0.f, 0.f, 0.f, 0.f };
	shadow_map_rt_desc.m_texture_desc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shadow_map_rt_desc.m_texture_desc.m_depth = 1;
	shadow_map_rt_desc.m_texture_desc.m_mipLevels = 1;
	shadow_map_rt_desc.m_texture_desc.m_sampleCount = SAMPLE_COUNT_1;
	shadow_map_rt_desc.m_texture_desc.m_width = m_moment_shadow_map_data.m_map_width;
	shadow_map_rt_desc.m_texture_desc.m_height = m_moment_shadow_map_data.m_map_height;
	shadow_map_rt_desc.m_texture_desc.m_isSRGB = false;

	m_moment_shadow_map_rt = DXResourceLoader::Create_RenderTarget(dxrenderer, shadow_map_rt_desc);

	BufferLoadDesc light_camera_uniform_buffer_desc = {};
	light_camera_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	light_camera_uniform_buffer_desc.m_desc. m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	light_camera_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	light_camera_uniform_buffer_desc.m_desc.m_debugName = "Light Camera Buffer";
	light_camera_uniform_buffer_desc.m_size = sizeof(CameraUniformData);
	light_camera_uniform_buffer_desc.m_rawData = nullptr;

	m_light_camera_uniform_buffer = DXResourceLoader::Create_Buffer(dxrenderer, light_camera_uniform_buffer_desc);

	TextureDesc intermediate_texture_desc = {};
	intermediate_texture_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE | Bind_Flags::BIND_UNORDERED_ACCESS;
	intermediate_texture_desc.m_clearVal = ClearValue{ 0.f, 0.f, 0.f, 0.f };
	intermediate_texture_desc. m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	intermediate_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	intermediate_texture_desc.m_width = m_moment_shadow_map_data.m_map_width;
	intermediate_texture_desc.m_height = m_moment_shadow_map_data.m_map_height;
	intermediate_texture_desc.m_depth = 1;
	intermediate_texture_desc.m_imageFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	intermediate_texture_desc.m_isSRGB = false;
	intermediate_texture_desc.m_mipLevels = 1;

	TextureLoadDesc intermediate_texture_load_desc = {};
	intermediate_texture_load_desc.m_tex_desc = &intermediate_texture_desc;

	m_intermediate_texture = DXResourceLoader::Create_Texture(m_dxrenderer, intermediate_texture_load_desc);
	m_output_texture = DXResourceLoader::Create_Texture(m_dxrenderer, intermediate_texture_load_desc);


	m_blur_image_pass.LoadContent(dxrenderer);
}

void MomentShadowMapRendering::RenderShadowMap()
{
	if (m_appRenderer->m_directionLightInstanceList.empty())
	{
		return;
	}

	DirectionalLightInstanceData& directional_light_inst_data = m_appRenderer->m_directionLightInstanceList[0];

	Vector3 light_view_pos = m_focusPoint + (directional_light_inst_data.light_direction * -65.f);
	Vector3 right_dir = directional_light_inst_data.light_direction.Cross(Vector3(0.f, 1.f, 0.f));
	Vector3 new_up_dir = right_dir.Cross(directional_light_inst_data.light_direction);
	new_up_dir.Normalize();

	m_shadow_camera_uniform_data.ViewMat = Matrix::CreateLookAt(light_view_pos,
		light_view_pos + directional_light_inst_data.light_direction, new_up_dir);

	static const float ortho_width = 75.f;
	static const float ortho_height = 75.f;

	Matrix shadow_orthographic_matrix = Matrix::CreateOrthographic(ortho_width, ortho_height, 0.1f, 100.f);

	m_shadow_camera_uniform_data.ProjectionMat = shadow_orthographic_matrix;
	m_shadow_camera_uniform_data.ViewProjectionMat = m_shadow_camera_uniform_data.ViewMat * m_shadow_camera_uniform_data.ProjectionMat;
	m_shadow_camera_uniform_data.CameraViewportSize.x = (float)m_moment_shadow_map_data.m_map_width;
	m_shadow_camera_uniform_data.CameraViewportSize.y = (float)m_moment_shadow_map_data.m_map_height;

	BufferUpdateDesc light_camera_buffer_update_desc = {};
	light_camera_buffer_update_desc.m_buffer = m_light_camera_uniform_buffer;
	light_camera_buffer_update_desc.m_pSource = &m_shadow_camera_uniform_data;
	light_camera_buffer_update_desc.m_size = sizeof(CameraUniformData);

	m_dxrenderer->cmd_update_buffer(light_camera_buffer_update_desc);

	DepthPassContext depth_pass_context(m_moment_shadow_map_rt, m_depth_rt,
		&m_shadow_camera_uniform_data, &m_appRenderer->m_basicInstances);

	m_depth_pass_rendering->RenderDepthPass(depth_pass_context);

	m_dxrenderer->cmd_bind_render_targets(nullptr, 0, nullptr, nullptr);

	m_blur_image_pass.SetBlurWidth(GraphicsSettings::Moment_Shadow_Map_Blur_Width);
	m_blur_image_pass.BlurImage(m_moment_shadow_map_rt->get_texture(), 
		m_intermediate_texture, m_output_texture);
}