#pragma once
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Graphics/InstanceRenderData.h"
#include "Helper/Hash.h"

class AppRenderer;

class TextRendering
{
public:
	friend class TextRenderingInstance;
public:
	TextRendering(AppRenderer* appRenderer);
	~TextRendering();

	void Initialize(DXRenderer* dxrenderer);
	void Release();
private:
	DXRenderer* m_dxrenderer;
	AppRenderer* m_appRenderer;
	std::unordered_map<FontType, StringId> m_fontTypeToResourceStringId;
};

