#include "BlendState.h"


BlendState::BlendState(const BlendStateDesc& blend_state_desc)
	:m_blend_state_desc(blend_state_desc), m_d3d_blend_state(nullptr)
{
}


BlendState::~BlendState()
{
}


void BlendState::Release()
{
	SafeRelease(m_d3d_blend_state);
}