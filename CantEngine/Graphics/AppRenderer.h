#pragma once

#include <vector>
#include "D3D11_Renderer.h"
#include "Renderer_Includes.h"
#include "Light.h"
#include "DebugRendering.h"
#include "DeferredRendering.h"
#include "Shaders/Shading.h"
#include "Graphics/MSAAResolvePass.h"
#include "Graphics/ParticleRendering.h"
#include "Graphics/InstanceRenderData.h"
#include "Graphics/AppRendererInstance.h"


class Material;
class Model;

struct SkyboxUniformData
{
	//no camera position included here
	Matrix ModelViewProjectionMat;
};

struct PointLightUniformData
{
	Vector4 LightPosition[MAX_LIGHT_COUNT];

	Vector4 LightColor[MAX_LIGHT_COUNT];

	//for now W is the brightness of the light,
	Vector4 LightMiscData[MAX_LIGHT_COUNT];

	//for now W is the total number of light
	Vector4 LightUniformMiscData;
};



class Light;


class ResourceManager;
class CameraManager;
class Camera;
class CameraRegistrationEvent;
class CameraDestructionEvent;
class WindowSizeEvent;



class AppRenderer{
public:
	friend class DeferredRendering;
	friend class DebugRendering;
	friend class DepthPassRendering;
	friend class MSAAResolvePass;
	friend class ShadowMapRendering;
	friend class ParticleRendering;
	friend class DeferredRenderingInstance;
	friend class DebugRenderingInstance;
	friend class AppRendererInstance;

	typedef std::vector<PointLightInstanceData> PointLightInstanceDataList;
	typedef std::vector<DirectionalLightInstanceData> DirectionalLightInstanceDataList;
public:
	AppRenderer(SDL_Window& sdlWindow, ResourceManager* resourceManager, 
		CameraManager* cameraManager);
	~AppRenderer();
	
	void UpdateAppRenderer(float dt);
	void RenderApp();
	void PresentApp();

	void Release();
	DXRenderer* GetDXRenderer();

	SDL_Window& GetSDLWindow();
	DebugRendering& GetDebugRendering();
	ParticleRendering& GetParticleRendering();


	void RegisterBasicInstance(const InstanceRenderData& instanceRenderData);
	void RegisterDirectionalLightInstance(const DirectionalLightInstanceData& directionalLightInstanceData);
	
	
	//void RenderBasicInstances(Pipeline* pipeline);

	void LoadContent();
private:
	void Initialize();
	void InnerLoadContent();

	void LoadSkyboxContent();
	void InitRandomTexture1D();
	void RenderSkybox();


	void AddObjectUniformBuffer(BufferList& objectUniformBufferList,
		std::vector<ObjectUniformData>& objectUniformDataList);
	void AddMaterialUniformBuffer();
	void UpdateMaterialUniformBuffer();

private:
	//Callbacks
	void OnCameraRegistration(const CameraRegistrationEvent* event);
	void OnCameraDestruction(const CameraDestructionEvent* event);
	void OnWindowSize(const WindowSizeEvent* event);

private:
	SDL_Window& m_sdl_window;
	DXRenderer* m_dxrenderer;
	ResourceManager* m_resourceManager;
	CameraManager* m_cameraManager;



	RasterizerState* m_cull_none_rasterizer_ms_state = nullptr;
	RasterizerState* m_cull_front_rasterizer_ms_state = nullptr;
	RasterizerState* m_cull_back_rasterizer_ms_state = nullptr;

	RasterizerState* m_cull_none_rasterizer_state = nullptr;
	RasterizerState* m_cull_front_rasterizer_state = nullptr;
	RasterizerState* m_cull_back_rasterizer_state = nullptr;

	RasterizerState* m_cull_none_wireframe_state = nullptr;

	DepthState* m_less_equal_depth_state = nullptr;
	DepthState* m_disabled_depth_state = nullptr;
	DepthState* m_testonlyLessEqualDepthState = nullptr;
	
	/*RenderTarget* m_cur_main_rt = nullptr;
	RenderTarget* m_msaa_main_rt = nullptr;
	RenderTarget* m_depth_rt = nullptr;*/

	Pipeline* m_skybox_pipeline;
	Shader* m_skybox_shader;
	Buffer* m_skybox_vertices_buffer;
	Texture* m_skybox_texture;


	Texture* m_random1DTexture;

	Sampler* m_clamp_linear_sampler = nullptr;
	Sampler* m_repeat_linear_sampler = nullptr;
	Sampler* m_trillinear_sampler;
	Sampler* m_texture_sampler;

	BlendState* m_blend_state_one_zero_add;
	BlendState* m_skybox_blend_state;
	BlendState* m_additiveBlending;


	
	float m_gameTime;
	ParticleRendering m_particleRendering;
	DebugRendering m_debugRendering;
	DeferredRendering m_deferrredRendering;
	MSAAResolvePass m_msaa_resolve_pass;

	Buffer* m_directional_light_uniform_buffer;
	DirectionalLightInstanceDataList m_directionLightInstanceList;
	PointLightInstanceDataList m_point_light_instance_list;

	
	
	InstanceRenderList m_basicInstances;

	std::map<int32_t, AppRendererInstance*> m_appRendererInstances;


	DirectionalLightUniformData m_directional_light_uniform_data;

	SkyboxUniformData m_skybox_uniform_data;

	
	std::vector<MaterialUniformData> m_material_uniform_data_list;

	Buffer* m_skybox_uniform_buffer;
	BufferList m_material_uniform_buffer_list;
	
};

