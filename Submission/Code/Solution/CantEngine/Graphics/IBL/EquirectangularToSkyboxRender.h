#pragma once
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class DXRenderer;
class AppRenderer;
class EquirectangularToSkyboxRenderInstance;


class EquirectangularToSkyboxRender
{
public:
	friend class EquirectangularToSkyboxRenderInstance;
public:
	EquirectangularToSkyboxRender(AppRenderer* appRenderer);
	~EquirectangularToSkyboxRender();

	void Release();

	void Render(Texture* src_texture, Texture* dest_texture);
	void Initialize(DXRenderer* dxrenderer);
	void ClearInstancesCount() { m_curInstancesCount = 0; };

	Pipeline* GetSkyboxPipeline()const 
	{
		return m_to_skybox_pipeline;
	};
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_to_skybox_pipeline;
	Shader* m_to_skybox_shader;

	uint32_t m_curInstancesCount;
	std::vector< EquirectangularToSkyboxRenderInstance* > m_instancesRender;
};


class EquirectangularToSkyboxRenderInstance
{
public:
	EquirectangularToSkyboxRenderInstance(EquirectangularToSkyboxRender& parent_render);

	void Render(Texture* src_texture, Texture* dest_texture);
	void Release();
private:
	void AddUniformBuffer();
private:
	EquirectangularToSkyboxRender& m_parent_render;
	std::vector<Buffer*> m_source_texture_to_skybox_uniform_buffer_list;
	std::vector<SourceTextureToSkyboxUniformData> m_source_texture_to_skybox_uniform_data_list;
};
