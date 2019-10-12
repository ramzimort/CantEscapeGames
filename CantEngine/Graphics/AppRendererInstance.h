#pragma once

#include "Renderer_Includes.h"
#include "Shaders/Shading.h"
class AppRendererInstance
{
public:
	AppRendererInstance();
	~AppRendererInstance();


	RenderTarget* m_cur_main_rt = nullptr;
	RenderTarget* m_msaa_main_rt = nullptr;
	RenderTarget* m_depth_rt = nullptr;
};

