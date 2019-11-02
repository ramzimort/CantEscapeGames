#pragma once
#include <DirectXTK/SpriteFont.h>

class DXFont
{
public:
	DXFont();
	~DXFont();

private:
	DirectX::SpriteFont* m_pSpriteFont;
};

