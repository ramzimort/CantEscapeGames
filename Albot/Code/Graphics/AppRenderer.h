#pragma once

#include <vector>
#include "D3D11_Renderer.h"
#include "Renderer_Includes.h"
#include "Light.h"
#include "DebugRendering.h"
#include "DeferredRendering.h"
#include "../Shaders/Shading.h"


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

struct PointLightInstanceData
{
	const Light* light;
	Vector3 light_position;
};

struct DirectionalLightInstanceData
{
	const Light* light;
	Vector3 light_direction;
};



struct InstanceRenderData
{
	Matrix model_mat = Matrix::Identity;
	Matrix normal_mat = Matrix::Identity;
	Material* p_ref_material = nullptr;
	Model* p_ref_model = nullptr;
	Vector2 uv_tiling = Vector2(1.0f, 1.0f);
};



class ResourceManager;
class CameraManager;

class AppRenderer{
public:
	friend class World;
	friend class DeferredRendering;
	friend class DebugRendering;
	friend class DepthPassRendering;

	typedef std::vector<Buffer*> BufferList;
	typedef std::vector<InstanceRenderData> InstanceRenderList;
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


	void RegisterBasicInstance(const InstanceRenderData& instanceRenderData);
	
	void RenderBasicInstances(Pipeline* pipeline);
private:
	void Initialize();
	void LoadContent();

	void LoadSkyboxContent();
	void RenderSkybox();


	void AddObjectUniformBuffer();
	void AddMaterialUniformBuffer();

private:
	SDL_Window& m_sdl_window;
	DXRenderer* m_dxrenderer;
	ResourceManager* m_resourceManager;
	CameraManager* m_cameraManager;


	RasterizerState* m_cull_none_rasterizer_state = nullptr;
	RasterizerState* m_cull_front_rasterizer_state = nullptr;
	RasterizerState* m_cull_back_rasterizer_state = nullptr;

	RasterizerState* m_cull_none_wireframe_state = nullptr;

	DepthState* m_less_equal_depth_state = nullptr;
	DepthState* m_disabled_depth_state = nullptr;

	RenderTarget* m_depth_rt = nullptr;
	Sampler* m_clamp_linear_sampler = nullptr;

	Pipeline* m_basic_pipeline;
	Shader* m_basic_shader;

	Pipeline* m_skybox_pipeline;
	Shader* m_skybox_shader;
	Buffer* m_skybox_vertices_buffer;
	Texture* m_skybox_texture;

	Sampler* m_trillinear_sampler;
	Sampler* m_texture_sampler;

	BlendState* m_blend_state_one_zero_add;
	BlendState* m_skybox_blend_state;


	Buffer* m_directional_light_uniform_buffer;
	DirectionalLightInstanceDataList m_direction_light_instance_list;
	PointLightInstanceDataList m_point_light_instance_list;

	DebugRendering m_debugRendering;
	DeferredRendering m_deferrredRendering;
	
	InstanceRenderList m_basicInstances;
	InstanceRenderList m_ui_quad_instances;

	CameraUniformData m_camera_uniform_data;
	PointLightUniformData m_light_uniform_data;

	DirectionalLightUniformData m_directional_light_uniform_data;

	SkyboxUniformData m_skybox_uniform_data;

	ObjectUniformData m_object_uniform_data;
	MaterialUniformData m_material_uniform_data;

	Buffer* m_point_light_uniform_buffer;
	Buffer* m_camera_uniform_buffer;
	Buffer* m_skybox_uniform_buffer;

	BufferList m_object_uniform_buffer_list;
	BufferList m_material_uniform_buffer_list;
	
};

