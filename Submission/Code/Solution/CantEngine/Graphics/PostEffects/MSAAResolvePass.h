#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"


class AppRenderer;
class MSAAResolvePass{
public:
	friend class MSAAResolvePassInstance;
public:
	MSAAResolvePass(AppRenderer* app_renderer);
	~MSAAResolvePass();

	void Release();

	void ResolveMSAASwapChain();

	void LoadContent(DXRenderer* dxrenderer);
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	Pipeline* m_resolve_pipeline = nullptr;
	Shader* m_resolve_shader = nullptr;
};

