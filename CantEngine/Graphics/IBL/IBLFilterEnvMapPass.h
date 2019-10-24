#pragma once
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class AppRenderer;

class IBLFilterEnvMapPass
{
public:
	IBLFilterEnvMapPass(AppRenderer* m_appRenderer);
	~IBLFilterEnvMapPass();

	void render(Texture* src_skybox_texture, Texture* dest_filtered_env_map);

	void Release();
	void initialize(DXRenderer* dxrenderer);
private:
	void add_uniform_buffer();
private:
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_ibl_filter_env_map_pipeline;
	Shader* m_ibl_filter_env_map_shader;


	std::vector<Buffer*> m_ibl_filter_env_map_uniform_buffer_list;
	std::vector<IBLFilterEnvMapUniformData> m_ibl_filter_env_map_uniform_data_list;
};

