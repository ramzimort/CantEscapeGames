#ifndef SHADER_H
#define SHADER_H

#include <string>


#include "Common_Renderer.h"

struct ShaderDesc
{
	std::string m_vertex_shader_path = "";
	std::string m_pixel_shader_path = "";
	std::string m_compute_shader_path = "";
};

struct ShaderLoadDesc
{
	ShaderDesc m_desc;
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
	void Inner_Initialize(ID3D11Device* device);
	void Initialize(ID3D11Device* device_context, 
		const std::string& vertex_path, const std::string& fragment_path);

private:
	ShaderDesc m_desc;
	uint32_t m_shader_stages;

	ID3D11VertexShader* m_vertex_shader;
	ID3DBlob* m_vertex_shader_blob;

	ID3D11PixelShader* m_pixel_shader;
	ID3DBlob* m_pixel_shader_blob;

	ID3D11ComputeShader* m_compute_shader;
	ID3DBlob* m_compute_shader_blob;
};

#endif