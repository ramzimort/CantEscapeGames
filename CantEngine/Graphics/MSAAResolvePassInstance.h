#pragma once
#include "Graphics/MSAAResolvePass.h"
#include "AppRendererInstance.h"

class MSAAResolvePassInstance
{
public:
	MSAAResolvePassInstance(MSAAResolvePass& msaaResolvePass);
	~MSAAResolvePassInstance();


	void Render(const AppRendererContext& appRendererContext);
	void Initialize(const AppRendererContext& appRendererContext);
	void LoadContent(const AppRendererContext& appRendererContext);
	void Release();
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	MSAAResolvePass& m_msaaResolvePass;

	Buffer* m_msaa_resolve_uniform_buffer;
	MSAAResolveUniformData m_msaa_resolve_uniform_data;
};

