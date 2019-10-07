#pragma once
#include "Sampler.h"


enum DepthWriteMask
{
	DONT_WRITE_DEPTH,
	WRITE_DEPTH
};

struct DepthStateDesc
{
	bool m_depth_enable;
	bool m_stencil_enable;
	DepthWriteMask m_depth_write_mask;
	CompareFunc m_depth_compare_func;
};

class DepthState
{
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	DepthState(const DepthStateDesc& desc);
	~DepthState();

	void Release();
private:
	DepthStateDesc m_desc;
	ID3D11DepthStencilState* m_p_depth_stencil_state;
};

