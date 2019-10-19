#pragma once
#include "Graphics/Particles/ParticleRendering.h"
#include "Graphics/AppRendererInstance.h"


class ParticleRenderingInstance
{
public:
	ParticleRenderingInstance(ParticleRendering& particleRendering);
	~ParticleRenderingInstance();

	void Render(const AppRendererContext& appRendererContext);
	void Initialize(const AppRendererContext& appRendererContext);
	void LoadContent(const AppRendererContext& appRendererContext);
	void Release();
private:
	void RenderParticles(const AppRendererContext& appRendererContext);

private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	ParticleRendering& m_particleRendering;
};

