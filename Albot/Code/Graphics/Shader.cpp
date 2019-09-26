#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "d3dUtils.h"



template<class ShaderClass >
std::string Get_Latest_Profile_Local(ID3D11Device* device);

template<class ShaderClass>
ShaderClass* Create_Shader_Local(ID3D11Device* device,
	ID3DBlob* p_shader_blob, ID3D11ClassLinkage* p_class_linkage);

template<>
std::string Get_Latest_Profile_Local<ID3D11VertexShader>(ID3D11Device* device)
{
	assert(device);

	D3D_FEATURE_LEVEL feature_level = device->GetFeatureLevel();

	switch (feature_level)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
	{
		return "vs_5_0";
	}
	break;
	case D3D_FEATURE_LEVEL_10_1:
	{
		return "vs_4_1";
	}
	break;
	case D3D_FEATURE_LEVEL_10_0:
	{
		return "vs_4_0";
	}
	break;
	case D3D_FEATURE_LEVEL_9_3:
	{
		return "vs_4_0_level_9_3";
	}
	break;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
	{
		return "vs_4_0_level_9_1";
	}
	break;
	}

	return "";
}

template<>
std::string Get_Latest_Profile_Local<ID3D11PixelShader>(ID3D11Device* device)
{
	assert(device);

	D3D_FEATURE_LEVEL feature_level = device->GetFeatureLevel();

	switch (feature_level)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
	{
		return "ps_5_0";
	}
	break;
	case D3D_FEATURE_LEVEL_10_1:
	{
		return "ps_4_1";
	}
	break;
	case D3D_FEATURE_LEVEL_10_0:
	{
		return "ps_4_0";
	}
	break;
	case D3D_FEATURE_LEVEL_9_3:
	{
		return "ps_4_0_level_9_3";
	}
	break;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
	{
		return "ps_4_0_level_9_1";
	}
	break;
	}

	return "";
}


template<>
ID3D11VertexShader* Create_Shader_Local<ID3D11VertexShader>(ID3D11Device* device,
	ID3DBlob* p_shader_blob, ID3D11ClassLinkage* p_class_linkage)
{
	assert(device);
	assert(p_shader_blob);

	ID3D11VertexShader* p_vertex_shader = nullptr;

	HRESULT hr = device->CreateVertexShader(
		p_shader_blob->GetBufferPointer(),
		p_shader_blob->GetBufferSize(),
		p_class_linkage, &p_vertex_shader);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	return p_vertex_shader;
}


template<>
ID3D11PixelShader* Create_Shader_Local<ID3D11PixelShader>(ID3D11Device* device,
	ID3DBlob* p_shader_blob, ID3D11ClassLinkage* p_class_linkage)
{
	assert(device);
	assert(p_shader_blob);

	ID3D11PixelShader* p_pixel_shader = nullptr;

	HRESULT hr = device->CreatePixelShader(
		p_shader_blob->GetBufferPointer(),
		p_shader_blob->GetBufferSize(),
		p_class_linkage, &p_pixel_shader);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	return p_pixel_shader;
}

template<class ShaderClass>
ShaderClass* Load_Shader(ID3D11Device* device,
	const std::string& file_name,
	const std::string& entry_point,
	const std::string& profile_args,
	uint32_t shader_macro_count,
	ShaderMacro* pp_shader_macro,
	ID3DBlob*& out_shader_blob)
{
	ID3DBlob* p_shader_blob = nullptr;
	ID3DBlob* p_error_blob = nullptr;
	ShaderClass* p_shader = nullptr;

	std::string profile = profile_args;
	if (profile == "latest")
	{
		profile = Get_Latest_Profile_Local<ShaderClass>(device);
	}

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	std::wstring file_name_w(file_name.begin(), file_name.end());

	D3D_SHADER_MACRO* d3d_shader_macro = (D3D_SHADER_MACRO*)alloca((shader_macro_count + 1) * sizeof(D3D_SHADER_MACRO));

	for (uint32_t i = 0; i < shader_macro_count; ++i)
	{
		d3d_shader_macro[i].Name = pp_shader_macro[i].m_name.c_str();
		d3d_shader_macro[i].Definition = pp_shader_macro[i].m_definition.c_str();
	}

	d3d_shader_macro[shader_macro_count] = { NULL, NULL };

	HRESULT hr = D3DCompileFromFile(
		file_name_w.c_str(),
		d3d_shader_macro,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry_point.c_str(), profile.c_str(),
		flags, 0, &p_shader_blob, &p_error_blob);

	if (FAILED_HR(hr))
	{
		if (p_error_blob)
		{
			std::string error_message =
				(char*)p_error_blob->GetBufferPointer();
			OutputDebugStringA(error_message.c_str());
			SafeRelease(p_shader_blob);
			SafeRelease(p_error_blob);
		}
		return nullptr;
	}

	p_shader = Create_Shader_Local<ShaderClass>(device,
		p_shader_blob, nullptr);

	out_shader_blob = p_shader_blob;

	//SafeRelease(p_shader_blob);
	SafeRelease(p_error_blob);

	return p_shader;
}


Shader::Shader(const ShaderDesc& desc)
	:m_desc(desc),
	m_shader_stages(Shader_Stages::UNDEFINED_STAGE),
	m_vertex_shader(nullptr),
	m_vertex_shader_blob(nullptr),
	m_pixel_shader(nullptr),
	m_pixel_shader_blob(nullptr),
	m_compute_shader(nullptr),
	m_compute_shader_blob(nullptr)
{}


Shader::~Shader()
{
}



void Shader::Inner_Initialize(ID3D11Device* device, const ShaderLoadDesc& shader_load_desc)
{
	Initialize(device, shader_load_desc);
}

void Shader::Initialize(ID3D11Device* device, const ShaderLoadDesc& shader_load_desc)
{
	if (shader_load_desc.m_desc.m_vertex_shader_path != "")
	{
		m_vertex_shader = Load_Shader<ID3D11VertexShader>(device,
			Constant::ShadersDir + shader_load_desc.m_desc.m_vertex_shader_path, "main", "latest",
			shader_load_desc.m_shader_macro_count, shader_load_desc.m_shader_macro,
			m_vertex_shader_blob);

		m_shader_stages |= Shader_Stages::VERTEX_STAGE;
	}

	if (shader_load_desc.m_desc.m_pixel_shader_path != "")
	{
		m_pixel_shader = Load_Shader<ID3D11PixelShader>(device,
			Constant::ShadersDir + shader_load_desc.m_desc.m_pixel_shader_path, "main", "latest",
			shader_load_desc.m_shader_macro_count, shader_load_desc.m_shader_macro,
			m_pixel_shader_blob);

		m_shader_stages |= Shader_Stages::PIXEL_STAGE;
	}
}


void Shader::Release()
{
	SafeRelease(m_vertex_shader_blob);
	SafeRelease(m_vertex_shader);

	SafeRelease(m_pixel_shader_blob);
	SafeRelease(m_pixel_shader);

	SafeRelease(m_compute_shader_blob);
	SafeRelease(m_compute_shader);
}