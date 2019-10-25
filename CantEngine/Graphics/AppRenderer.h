#pragma once

#include <vector>
#include "D3D11_Renderer.h"
#include "Renderer_Includes.h"
#include "Light.h"
#include "Graphics/DebugRendering/DebugRendering.h"
#include "Graphics/Deferred/DeferredRendering.h"
#include "Shaders/Shading.h"
#include "Graphics/Shadows/MomentShadowMapRendering.h"
#include "Graphics/PostEffects/MSAAResolvePass.h"
#include "Graphics/Particles/ParticleRendering.h"
#include "Graphics/InstanceRenderData.h"
#include "Graphics/AppRendererInstance.h"
#include "Graphics/IBL/EquirectangularToSkyboxRender.h"
#include "Graphics/IBL/IBLFilterEnvMapPass.h"
#include "Graphics/IBL/BRDFLookupTexturePass.h"

class Material;
class Model;

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
	friend class MomentShadowMapRendering;
	friend class MSAAResolvePass;
	friend class ParticleRendering;
	friend class DeferredRenderingInstance;
	friend class DebugRenderingInstance;
	friend class ParticleRenderingInstance;
	friend class AppRendererInstance;
	friend class EquirectangularToSkyboxRender;
	friend class EquirectangularToSkyboxRenderInstance;
	friend class IBLFilterEnvMapPass;
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
	void RegisterHaloEffectInstance(const HaloEffectInstanceData& haloEffectData);
	void RegisterPointLightInstance(const PointLightInstanceData& pointLightInstanceData);
	void RegisterBoneMeshInstance(const BoneMeshInstanceRenderData& boneMeshInstanceData);

	void RegisterProcessSkyboxIrradianceInstance(const ProcessSkyboxIrradianceInstanceData& processInstanceData);
	void RegisterBakedSkyboxIrradianceInstance(const BakedSkyboxIrradianceInstanceData& bakedInstanceData);

	void InitializeRenderer();
	void InitializeResources();
	void LoadContent();
private:
	
	void InitializeDefaultIBLData();
	void InnerLoadContent();

	void InitializeSkyboxData();
	void InitRandomTexture1D();
	void RenderSkybox();


	void AddObjectUniformBuffer(BufferList& objectUniformBufferList,
		std::vector<ObjectUniformData>& objectUniformDataList);
	void AddMaterialUniformBuffer();
	void UpdateMaterialUniformBuffer();

	void BakeSkyboxIrradianceData();
private:
	//Callbacks
	void OnCameraRegistration(const CameraRegistrationEvent* event);
	void OnCameraDestruction(const CameraDestructionEvent* event);
	void OnWindowSize(const WindowSizeEvent* event);


	void ResolveAppRendererInstances();


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
	

	Pipeline* m_resolveAppRendererInstancesPipeline = nullptr;
	Shader* m_resolveAppRendererInstancesShader = nullptr;


	Pipeline* m_skybox_pipeline;
	Shader* m_skybox_shader;
	Buffer* m_skybox_vertices_buffer;


	Texture* m_random1DTexture;

	Sampler* m_clamp_linear_sampler = nullptr;
	Sampler* m_repeat_linear_sampler = nullptr;
	Sampler* m_trillinear_sampler;
	Sampler* m_texture_sampler;

	BlendState* m_blend_state_one_zero_add;
	BlendState* m_skybox_blend_state;
	BlendState* m_additiveBlending;


	Texture* m_pBRDFLookupTexture;
	EquirectangularToSkyboxRender m_toSkyboxRender;
	IBLFilterEnvMapPass m_iblFilterEnvMapPass;
	BRDFLookupTexturePass m_brdfLookupTexturePass;

	ProcessSkyboxIrradianceInstanceDataList m_processSkyboxIrradianceInstanceList;
	BakedSkyboxIrradianceInstanceDataList m_bakedSkyboxIrradianceInstanceList;
	
	float m_gameTime;
	ParticleRendering m_particleRendering;
	DebugRendering m_debugRendering;
	DeferredRendering m_deferrredRendering;
	MomentShadowMapRendering m_momentShadowMapRendering;
	MSAAResolvePass m_msaa_resolve_pass;


	Buffer* m_directional_light_uniform_buffer;
	DirectionalLightInstanceDataList m_directionLightInstanceList;
	PointLightInstanceDataList m_pointLightInstanceList;

	HaloEffectInstanceList m_haloEffectInstanceList;
	
	InstanceRenderList m_basicInstances;
	BoneMeshInstancesRenderDataList m_boneMeshInstancesList;

	std::map<int32_t, AppRendererInstance*> m_appRendererInstances;
	DirectionalLightUniformData m_directional_light_uniform_data;

	std::vector<MaterialUniformData> m_material_uniform_data_list;
	BufferList m_material_uniform_buffer_list;



	
};

