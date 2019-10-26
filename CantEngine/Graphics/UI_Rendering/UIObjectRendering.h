#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class AppRenderer;


class UIObjectRendering
{
public:
	friend class UIObjectRenderingInstance;
public:
	UIObjectRendering(AppRenderer* appRenderer);
	~UIObjectRendering();

	void Initialize(DXRenderer* dxrenderer);
	void Release();
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_pUIObjectRenderingPipeline;
	Shader* m_pUIObjectRenderingShader;
};

