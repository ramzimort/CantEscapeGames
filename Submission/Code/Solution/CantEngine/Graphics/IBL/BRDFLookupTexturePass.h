#pragma once
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class AppRenderer;

class BRDFLookupTexturePass
{
public:
	BRDFLookupTexturePass(AppRenderer* appRenderer);
	~BRDFLookupTexturePass();


	void Render(Texture* brdf_lut_texture);

	void Initialize(DXRenderer* dxrenderer);
	void Release();
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_brdf_lookup_texture_pass_pipeline;
	Shader* m_brdf_lookup_texture_pass_shader;

	ProcessBRDFLookupTextureUniformData m_process_brdf_lookup_texture_uniform_data;
	Buffer* m_process_brdf_lookup_texture_uniform_buffer;

};

