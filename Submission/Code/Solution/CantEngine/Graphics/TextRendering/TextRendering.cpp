#include "stdafx.h"
#include "TextRendering.h"


TextRendering::TextRendering(AppRenderer* appRenderer)
	:m_appRenderer(appRenderer)
{
}


TextRendering::~TextRendering()
{
}


void TextRendering::Initialize(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;
	m_fontTypeToResourceStringId[FontType::COURIER_NEW] = "Assets\\Fonts\\courier_new.spritefont";
	m_fontTypeToResourceStringId[FontType::COURIER_NEW_BOLD] = "Assets\\Fonts\\courier_new_bold.spritefont";
}

void TextRendering::Release()
{
}
