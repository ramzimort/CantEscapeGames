#include "RasterizerState.h"


RasterizerState::RasterizerState(const RasterizerStateDesc& desc)
	:m_desc(desc)
{
}


RasterizerState::~RasterizerState()
{
}


void RasterizerState::Release()
{
	SafeRelease(m_d3d_rasterizer_state);
}