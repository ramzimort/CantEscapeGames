#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class DepthPassRendering;
class DXRenderer;
class AppRenderer;

struct ShadowMapData
{
	uint32_t m_map_width;
	uint32_t m_map_height;
};

class ShadowMapRendering
{
public:
	ShadowMapRendering(AppRenderer* app_renderer, const ShadowMapData& shadow_map_data);
	~ShadowMapRendering();

	void render_shadow_map();
	void Release();
	void load_content(DXRenderer* dxrenderer);

	RenderTarget*& get_shadow_map_rt();
	Buffer* get_light_camera_uniform_buffer() const { return m_light_camera_uniform_buffer; };

private:
	DepthPassRendering* m_depth_pass_rendering;
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	RenderTarget* m_shadow_map_rt;

	ShadowMapData m_shadow_map_data;
	CameraUniformData m_shadow_camera_uniform_data;
	Buffer* m_light_camera_uniform_buffer;
};

