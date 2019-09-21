#pragma once

enum FillMode
{
	SOLID,
	WIREFRAME,
	FILL_MODE_MAX_COUNTS
};

enum CullMode
{
	NONE,
	BACK,
	FRONT,
	CULL_MODE_MAX_COUNTS
};

struct RasterizerStateDesc
{
	FillMode m_fill_mode;
	CullMode m_cull_mode;
};

class RasterizerState
{
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	RasterizerState(const RasterizerStateDesc& desc);
	~RasterizerState();

	void Release();
private:
	RasterizerStateDesc m_desc;
	ID3D11RasterizerState* m_d3d_rasterizer_state;
};

