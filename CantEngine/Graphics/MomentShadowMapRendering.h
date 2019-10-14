#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/MomentShadowMapRendering.h"
#include "Graphics/BlurImagePass.h"

class DepthPassRendering;
class DXRenderer;
class AppRenderer;

struct MomentShadowMapData
{
	uint32_t m_map_width;
	uint32_t m_map_height;
};


class MomentShadowMapRendering
{
public:
	MomentShadowMapRendering(AppRenderer* app_renderer, const MomentShadowMapData& shadow_map_data);
	~MomentShadowMapRendering();

	void RenderShadowMap();
	void Release();
	void LoadContent(DXRenderer* dxrenderer);
	void SetFocusPoint(const Vector3& focusPoint);

	Texture* get_blurred_shadow_map_texture() const { return m_output_texture; };
	RenderTarget*& get_shadow_map_rt();
	Buffer* get_light_camera_uniform_buffer() const { return m_light_camera_uniform_buffer; };

private:
	DepthPassRendering* m_depth_pass_rendering;
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;
	RenderTarget* m_moment_shadow_map_rt;
	RenderTarget* m_depth_rt;

	MomentShadowMapData m_moment_shadow_map_data;
	CameraUniformData m_shadow_camera_uniform_data;
	Buffer* m_light_camera_uniform_buffer;

	Texture* m_intermediate_texture;
	Texture* m_output_texture;

	BlurImagePass m_blur_image_pass;
	Vector3 m_focusPoint;
};

