#include "DepthState.h"


DepthState::DepthState(const DepthStateDesc& desc)
	:m_desc(desc),
	m_p_depth_stencil_state(nullptr)
{
}


DepthState::~DepthState()
{
}


void DepthState::Release()
{
	SafeRelease(m_p_depth_stencil_state);
}