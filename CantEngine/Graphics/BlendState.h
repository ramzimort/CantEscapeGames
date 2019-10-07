#pragma once
#include "Common_Renderer.h"

enum BlendFactor
{
	BF_ZERO = 0,
	BF_ONE,
	BF_SRC_COLOR,
	BF_ONE_MINUS_SRC_COLOR,
	BF_DST_COLOR,
	BF_ONE_MINUS_DST_COLOR,
	BF_SRC_ALPHA,
	BF_ONE_MINUS_SRC_ALPHA,
	BF_DST_ALPHA,
	BF_ONE_MINUS_DST_ALPHA,
	BF_SRC_ALPHA_SATURATE,
	BF_BLEND_FACTOR,
	BF_INV_BLEND_FACTOR,
	BF_TOTAL_COUNTS
};

enum BlendOperator
{
	BO_ADD,
	BO_SUBTRACT,
	BO_REVERSE_SUBTRACT,
	BO_MIN,
	BO_MAX,
	BO_TOTAL_COUNTS
};

enum BlendStateTargets
{
	BLEND_STATE_RT_0 = 0x01,
	BLEND_STATE_RT_1 = 0x02,
	BLEND_STATE_RT_2 = 0x04,
	BLEND_STATE_RT_3 = 0x08,
	BLEND_STATE_RT_4 = 0x10,
	BLEND_STATE_RT_5 = 0x20,
	BLEND_STATE_RT_6 = 0x40,
	BLEND_STATE_RT_7 = 0x80,
	BLEND_STATE_RT_ALL = 0xFF
};

struct BlendStateDesc
{
	BlendFactor m_srcFactors[MAX_RENDER_TARGET_ATTACHMENTS];
	BlendFactor m_dstFactors[MAX_RENDER_TARGET_ATTACHMENTS];

	BlendFactor m_srcAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];
	BlendFactor m_dstAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];

	BlendOperator m_blendOperator[MAX_RENDER_TARGET_ATTACHMENTS];
	BlendOperator m_blendAlphaOperator[MAX_RENDER_TARGET_ATTACHMENTS];
	BlendStateTargets m_blendStateTarget;

	bool m_enableAlphaCoverage;
	bool m_individualBlend;
};

class BlendState
{
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	BlendState(const BlendStateDesc& blend_state_desc);
	~BlendState();

	void Release();
private:
	ID3D11BlendState* m_d3d_blend_state;
	BlendStateDesc m_blend_state_desc;
};

