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

	void render(Texture* src_texture, Texture* dest_texture);
	void initialize(DXRenderer* dxrenderer);
	void clear_instances_count() { m_cur_instances_count = 0; };

	Pipeline* get_skybox_pipeline()const 
	{
		return m_to_skybox_pipeline;
	};
private:
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_to_skybox_pipeline;
	Shader* m_to_skybox_shader;

	uint32_t m_cur_instances_count;
	std::vector< EquirectangularToSkyboxRenderInstance* > m_instances_render;

	/*Buffer* m_source_texture_to_skybox_uniform_buffer;
	SourceTextureToSkyboxUniformData m_source_texture_to_skybox_uniform_data;*/
	
};


class EquirectangularToSkyboxRenderInstance
{
public:
	EquirectangularToSkyboxRenderInstance(EquirectangularToSkyboxRender& parent_render);

	void render(Texture* src_texture, Texture* dest_texture);
	void Release();
private:
	void add_uniform_buffer();
private:
	EquirectangularToSkyboxRender& m_parent_render;
	std::vector<Buffer*> m_source_texture_to_skybox_uniform_buffer_list;
	std::vector<SourceTextureToSkyboxUniformData> m_source_texture_to_skybox_uniform_data_list;
};
