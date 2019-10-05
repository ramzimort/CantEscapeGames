#ifndef SHADER_H
#define SHADER_H

#include <string>


#include "Common_Renderer.h"

struct ShaderMacro
{
	std::string m_name = "";
	std::string m_definition = "";
};

struct ShaderDesc
{
	std::string m_vertex_shader_path = "";
	std::string m_pixel_shader_path = "";
	std::string m_compute_shader_path = "";
	std::string m_geometry_shader_path = "";
};

struct ShaderLoadDesc
{
	ShaderDesc m_desc;
	uint32_t m_shader_macro_count = 0;
	ShaderMacro* m_shader_macro;
};



class Shader {
public:
	friend class DXResourceLoader;
	friend class Pipeline;
	friend class DXRenderer;
public:
	Shader(const ShaderDesc& desc);
	~Shader();


	void Release();
private:
	void Inner_Initialize(ID3D11Device* device, const ShaderLoadDesc& shader_load_desc);
	void Initialize(ID3D11Device* device_context, const ShaderLoadDesc& shader_load_desc);

private:
	ShaderDesc m_desc;
	uint32_t m_shader_stages;

	ID3D11VertexShader* m_vertex_shader;
	ID3DBlob* m_vertex_shader_blob;

	ID3D11PixelShader* m_pixel_shader;
	ID3DBlob* m_pixel_shader_blob;

	ID3D11ComputeShader* m_compute_shader;
	ID3DBlob* m_compute_shader_blob;

	ID3D11GeometryShader* m_geometry_shader;
	ID3DBlob* m_geometry_shader_blob;
};

#endif