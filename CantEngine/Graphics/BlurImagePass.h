#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

class AppRenderer;

enum BlurChannel
{
	BLUR_CHANNEL_ONE,
	BLUR_CHANNEL_TWO,
	BLUR_CHANNEL_THREE,
	BLUR_CHANNEL_FOUR
};


class BlurImagePass
{
public:
	BlurImagePass(AppRenderer* app_renderer, 
		BlurChannel blur_channel = BLUR_CHANNEL_ONE, uint32_t blur_width = 8);
	~BlurImagePass();

	void Release();
	void BlurImage(Texture* input_texture, Texture* intermediate_texture, Texture* output_texture_to_blur);

	void LoadContent(DXRenderer* dxrenderer);
	void SetBlurWidth(uint32_t blur_width);
private:
	void BlurHorizontal(Texture* input_texture, Texture* output_texture_to_blur);
	void BlurVertical(Texture* input_texture, Texture* output_texture_to_blur);
private:
	uint32_t m_blur_width;

	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_blur_pipeline;
	Shader* m_blur_shader;

	Buffer* m_horizontal_blur_uniform_buffer;
	Buffer* m_vertical_blur_uniform_buffer;

	BlurChannel m_blur_channel_type;
	BlurUniformData m_vertical_blur_uniform_data;
	BlurUniformData m_horizontal_blur_uniform_data;
};

