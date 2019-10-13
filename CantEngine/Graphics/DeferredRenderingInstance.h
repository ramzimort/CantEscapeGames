#pragma once
#include "Graphics/DeferredRendering.h"
#include "AppRendererInstance.h"

class DeferredRenderingInstance
{
public:
	DeferredRenderingInstance(DeferredRendering& deferredRendering);
	~DeferredRenderingInstance();

	void Render(const AppRendererContext& appRendererContext);
	void Initialize(const AppRendererContext& appRendererContext);
	void LoadContent(const AppRendererContext& appRendererContext);
	void Release();
private:
	void RenderDeferredPass(const AppRendererContext& appRendererContext);
	void RenderDeferredGlobalDirectionalLightShade(const AppRendererContext& appRendererContext);
	void RenderDeferredPointLightShade(const AppRendererContext& appRendererContext);
private:
	DeferredRendering& m_deferredRendering;
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	RenderTarget* m_deferred_rts[DEFERRED_TOTAL_COUNT];
};

