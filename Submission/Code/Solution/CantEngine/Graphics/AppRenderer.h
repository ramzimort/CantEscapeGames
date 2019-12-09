#pragma once
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Graphics/Light.h"
#include "Graphics/DebugRendering/DebugRendering.h"
#include "Graphics/Deferred/DeferredRendering.h"
#include "Graphics/Shadows/MomentShadowMapRendering.h"
#include "Graphics/PostEffects/MSAAResolvePass.h"
#include "Graphics/Particles/ParticleRendering.h"
#include "Graphics/InstanceRenderData.h"
#include "Graphics/AppRendererInstance.h"
#include "Graphics/IBL/EquirectangularToSkyboxRender.h"
#include "Graphics/IBL/IBLFilterEnvMapPass.h"
#include "Graphics/IBL/BRDFLookupTexturePass.h"
#include "Graphics/UI_Rendering/UIObjectRendering.h"
#include "Graphics/TextRendering/TextRendering.h"
#include "Shaders/Shading.h"



/**
 * @file AppRenderer.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public high level interface renderer available for users to use
 */

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
/**
 * @brief The high level renderer interface
 * 
 */
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
	friend class UIObjectRendering;
	friend class UIObjectRenderingInstance;
	friend class TextRendering;
	friend class TextRenderingInstance;
public:
	/**
	* @brief Construct a new App Renderer object
	* 
	* @param sdlWindow 
	* @param resourceManager 
	* @param cameraManager 
	*/
	AppRenderer(SDL_Window& sdlWindow, ResourceManager* resourceManager, 
		CameraManager* cameraManager);
	~AppRenderer();
	
	/**
	 * @brief Contains update logic for AppRenderer. No render api is called in this function
	 * 
	 * @param dt 
	 */
	void UpdateAppRenderer(float dt);

	/**
	 * @brief Render all queued render request info to swap chain
	 * 
	 */
	void RenderApp();

	/**
	 * @brief Present rendered data to swapchain
	 * 
	 */
	void PresentApp();

	/**
	 * @brief Release all graphics related resources
	 * 
	 */
	void Release();

	/**
	 * @brief Get DXRenderer, a D3D11 interface renderer
	 * 
	 * @return DXRenderer* 
	 */
	DXRenderer* GetDXRenderer();

	/**
	 * @brief Get reference to SDLWindow
	 * 
	 * @return SDL_Window& 
	 */
	SDL_Window& GetSDLWindow();

	/**
	 * @brief Get the Debug Rendering object
	 * 
	 * @return DebugRendering& 
	 */
	DebugRendering& GetDebugRendering();

	/**
	 * @brief Get the Particle Rendering object
	 * 
	 * @return ParticleRendering& 
	 */
	ParticleRendering& GetParticleRendering();

	/**
	 * @brief Pass render info request for any primitive meshes or artist defined meshes
	 * 
	 * @param instanceRenderData 
	 */
	void RegisterBasicInstance(const InstanceRenderData& instanceRenderData);

	/**
	 * @brief Pass directional lighting info to the current scene. Used to illuminate scene globally
	 * 
	 * @param directionalLightInstanceData 
	 */
	void RegisterDirectionalLightInstance(const DirectionalLightInstanceData& directionalLightInstanceData);

	/**
	 * @brief Pass halo effect info to the current scene. Sphere-like glow effect
	 * 
	 * @param haloEffectData 
	 */
	void RegisterHaloEffectInstance(const HaloEffectInstanceData& haloEffectData);

	/**
	 * @brief Pass point lighting info to the current scene. Used to illuminate portion of the scene
	 * 
	 * @param pointLightInstanceData 
	 */
	void RegisterPointLightInstance(const PointLightInstanceData& pointLightInstanceData);

	/**
	 * @brief Pass render info request for animated skinned model.
	 * 
	 * @param boneMeshInstanceData 
	 */
	void RegisterBoneMeshInstance(const BoneMeshInstanceRenderData& boneMeshInstanceData);


	/**
	 * @brief Pass render info request for orthographic UI rendering
	 * 
	 * @param uiObjectInstanceData 
	 */
	void RegisterUIObjectInstance(const UIObjectInstanceRenderData& uiObjectInstanceData);

	/**
	 * @brief Pass render info request for DX11 screen space text rendering
	 * 
	 * @param textFontInstanceData 
	 */
	void RegisterTextFontInstance(const TextFontInstanceRenderData& textFontInstanceData);

	/**
	 * @brief Pass render info request for DX11 screen space text rendering
	 * 
	 * @param text 
	 * @param fontType 
	 * @param position 
	 * @param color 
	 * @param scale 
	 * @param rotation 
	 */
	void RegisterTextFontInstance(const std::string& text, uint32_t fontType,
		const Vector2& position, const Vector3& color, const Vector3& scale, float rotation);
	
	/**
	 * @brief Pass render info request for DX11 screen space text rendering
	 * 
	 * @param text 
	 * @param fontType 
	 * @param position 
	 * @param color 
	 * @param scale 
	 * @param rotation 
	 */
	void RegisterTextFontInstance(const std::wstring& text, FontType fontType, 
		const Vector2& position, const Vector3& color, const Vector3& scale, float rotation);

	/**
	 * @brief Pass unbaked skybox data to the internal app renderer. App rendere bake the skybox to irradiance & specular data
	 * 
	 * @param processInstanceData 
	 */
	void RegisterProcessSkyboxIrradianceInstance(const ProcessSkyboxIrradianceInstanceData& processInstanceData);

	/**
	 * @brief Pass baked skybox irradiance data to the scene. Will illuminate the scene globally based on skybox HDR illumination
	 * 
	 * @param bakedInstanceData 
	 */
	void RegisterBakedSkyboxIrradianceInstance(const BakedSkyboxIrradianceInstanceData& bakedInstanceData);
	/**
	 * @brief Initialize DX11 renderer
	 * 
	 */
	void InitializeRenderer();
	/**
	 * @brief Initialize high level resources
	 * 
	 */
	void InitializeResources();

	/**
	 * @brief Load high level resources (this is set to be called for every windows resize event in the future)
	 * 
	 */
	void LoadContent();

	/**
	 * @brief Get the Moment Shadow Map Rendering object
	 *
	 * @return MomentShadowMapRendering&
	 */
	MomentShadowMapRendering& GetMomentShadowMap();
private:
	
	void InitializeDefaultIBLData();
	void InnerLoadContent();

	void InitializeSkyboxData();
	void InitRandomTexture1D();
	void RenderSkybox();

	void UpdateBoneTransformsBufferList();
	void AddBoneTransformBuffer();
	void AddObjectUniformBuffer(BufferList& objectUniformBufferList,
		std::vector<ObjectUniformData>& objectUniformDataList);
	void AddObjectUniformBuffer(BuffersDeque& objectUniformBufferDeques,
		std::deque <ObjectUniformData>& objectUniformDataDeque);
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
	BlendState* m_uiTransparencyBlending;


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

	std::deque<MaterialUniformData> m_material_uniform_data_list;
	std::deque<Material*> m_materialReferenceList;
	BufferList m_material_uniform_buffer_list;

	std::deque<BoneTransformsUniformData> m_boneTransformsUniformDataList;
	std::deque<Buffer*> m_boneTransformsUniformBufferList;


	UIObjectInstanceRenderDataList m_uiObjectInstanceRenderDataList;
	UIObjectRendering m_uiObjectRendering;


	TextFontInstanceRenderDataList m_textFontInstanceRenderDataList;
	TextRendering m_textRendering;
};

