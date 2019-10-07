#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"


class AppRenderer;
class MSAAResolvePass{
public:
	MSAAResolvePass(AppRenderer* app_renderer);
	~MSAAResolvePass();

	void Release();

	void ResolveMSAASwapChain();

	void LoadContent(DXRenderer* dxrenderer);
private:
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;

	Buffer* m_msaa_resolve_uniform_buffer;
	MSAAResolveUniformData m_msaa_resolve_uniform_data;

	

	Pipeline* m_resolve_pipeline = nullptr;
	Shader* m_resolve_shader = nullptr;
};

