#pragma once

enum CompareFunc
{
	CMP_NEVER,
	CMP_LESS,
	CMP_EQUAL,
	CMP_LEQUAL,
	CMP_GREATER,
	CMP_NOTEQUAL,
	CMP_GEQUAL,
	CMP_ALWAYS,
	MAX_COMPARE_FUNCS
};


enum SamplerFilterType
{
	FILTER_POINT,
	FILTER_LINEAR
};

enum SamplerAddressMode
{
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_REPEAT,
	ADDRESS_MODE_CLAMP_TO_EDGE,
	ADDRESS_MODE_CLAMP_TO_BORDER
};

enum SamplerMipMapMode
{
	MIPMAP_MODE_POINT,
	MIPMAP_MODE_LINEAR
};


struct SamplerDesc
{
	SamplerFilterType m_min_filter;
	SamplerFilterType m_mag_filter;
	SamplerMipMapMode m_mipmap_mode;
	SamplerAddressMode m_address_u;
	SamplerAddressMode m_address_v;
	SamplerAddressMode m_address_w;
	float m_mip_los_bias;
	float m_max_aniso;
	CompareFunc m_compare_func;
};


class Sampler
{
public:
	friend class DXRenderer;
	friend class DXResourceLoader;
public:
	Sampler(const SamplerDesc& sampler_desc);
	~Sampler();

	void Release();
private:
	SamplerDesc m_desc;
	ID3D11SamplerState* m_sampler_state;
};

