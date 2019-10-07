#pragma once

#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/Light.h"


class DXRenderer;
class AppRenderer;
class ResourceManager;

enum DEFERRED_RT_TYPE
{
	DEFERRED_WORLD_NORMAL,
	DEFERRED_ALBEDO,
	DEFERRED_SPECULAR,
	DEFERRED_TOTAL_COUNT
};

class DeferredRendering
{
public:
	DeferredRendering(AppRenderer* app_renderer, ResourceManager* resourceManager);
	~DeferredRendering();

	void Release();
	void RenderDeferredScene();
	void LoadContent(DXRenderer* dxrenderer);
private:
	void render_deferred_pass();
	void render_deferred_global_directional_light_shade();
	void render_deferred_point_light_shade();
	void init_deferred_scene_light();
private:
	DXRenderer* m_dxrenderer;
	AppRenderer* m_app_renderer;

	Pipeline* m_deferred_pass_pipeline;
	Shader* m_deferred_pass_shader;
	RenderTarget* m_deferred_rts[DEFERRED_TOTAL_COUNT];

	Pipeline* m_deferred_shade_pipeline;
	Shader* m_deferred_shade_shader;


	Pipeline* m_deferred_shade_pointlight_pipeline;
	Shader* m_deferred_shade_pointlight_shader;
	Buffer* m_point_light_buffer;
	ConstantPointLightData m_constant_point_light_data[MAX_DEFERRED_POINT_LIGHT];

	Light m_point_light_deferred_scene[900];

	ResourceManager* m_resourceManager;
};

