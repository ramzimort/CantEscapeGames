#include "stdafx.h"
#include "UIObjectRenderingInstance.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Material.h"
#include "Managers/ResourceManager.h"

static const std::string QuadModelDir = Constant::ModelsDir + "plane.fbx";

UIObjectRenderingInstance::UIObjectRenderingInstance(UIObjectRendering& uiObjectRendering)
	:m_uiObjectRendering(uiObjectRendering)
{
	m_appRenderer = m_uiObjectRendering.m_appRenderer;
	m_dxrenderer = m_uiObjectRendering.m_dxrenderer;
}


UIObjectRenderingInstance::~UIObjectRenderingInstance()
{
}

void UIObjectRenderingInstance::Initialize(const AppRendererContext& appRendererContext)
{

}

void UIObjectRenderingInstance::LoadContent(const AppRendererContext& appRendererContext)
{

}

void UIObjectRenderingInstance::Release()
{
	for (Buffer* buffer : m_uiObjectUniformBufferList)
	{
		SafeReleaseDelete(buffer);
	}
}

void UIObjectRenderingInstance::Render(const AppRendererContext& appRendererContext)
{
	if (m_appRenderer->m_uiObjectInstanceRenderDataList.empty())
	{
		return;
	}

	Model* quadModel = m_appRenderer->m_resourceManager->GetModel(QuadModelDir);

	for (uint64_t index = 0; index < m_appRenderer->m_uiObjectInstanceRenderDataList.size(); ++index)
	{
		const UIObjectInstanceRenderData& uiObjectInstance = m_appRenderer->m_uiObjectInstanceRenderDataList[index];

		assert(uiObjectInstance.m_pUIMaterial != nullptr);
		Material* pUIMaterial = uiObjectInstance.m_pUIMaterial;

		//add by half the size of UI so that the origin is top left
		Vector2 finalUIPosition = uiObjectInstance.m_windowSpacePosition + 
			Vector2(uiObjectInstance.m_windowSpaceSize.x / 2.f, uiObjectInstance.m_windowSpaceSize.y / 2.f);

		Vector2 finalUIScale = uiObjectInstance.m_windowSpaceSize;

		if (index >= m_appRenderer->m_uiObjectInstanceRenderDataList.size())
		{
			AddUIObjectUniformBuffer();
		}

		m_uiObjectUniformDataList[index].ModelMat = Matrix::CreateScale(Vector3(finalUIScale.x, finalUIScale.y, 1.f))
			* Matrix::CreateTranslation(Vector3(finalUIPosition.x, finalUIPosition.y, -0.1f));

		m_uiObjectUniformDataList[index].ModelViewProjectionMat = m_uiObjectUniformDataList[index].ModelMat * 
			appRendererContext.m_appRendererInstance->m_camera_uniform_data.ViewProjectionMat;

		Texture* pUITexture = pUIMaterial->GetDiffuseTexture();

		assert(pUITexture);

		BufferUpdateDesc updateUIObjectUniformDesc = {};
		updateUIObjectUniformDesc.m_buffer = m_uiObjectUniformBufferList[index];
		updateUIObjectUniformDesc.m_pSource = &m_uiObjectUniformDataList[index];
		updateUIObjectUniformDesc.m_size = sizeof(UIObjectUniformData);
		m_dxrenderer->cmd_update_buffer(updateUIObjectUniformDesc);

		uint32_t paramsCount = 1;

		DescriptorData params[3] = {};
		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
		params[0].m_buffers = &m_uiObjectUniformBufferList[index];

		if (pUITexture)
		{
			params[1].m_binding_location = 0;
			params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_SAMPLER;
			params[1].m_shader_stages = Shader_Stages::PIXEL_STAGE;
			params[1].m_samplers = &m_appRenderer->m_texture_sampler;

			params[2].m_binding_location = 0;
			params[2].m_descriptor_type = DescriptorType::DESCRIPTOR_TEXTURE;
			params[2].m_shader_stages = Shader_Stages::PIXEL_STAGE;
			params[2].m_textures = &pUITexture;

			paramsCount = 3;
		}

		m_dxrenderer->cmd_bind_pipeline(m_uiObjectRendering.m_pUIObjectRenderingPipeline);
		m_dxrenderer->cmd_bind_vertex_buffer(quadModel->GetVertexBuffer());
		m_dxrenderer->cmd_bind_index_buffer(quadModel->GetIndexBuffer());
		m_dxrenderer->cmd_bind_descriptor(m_uiObjectRendering.m_pUIObjectRenderingPipeline, paramsCount, params);
		m_dxrenderer->cmd_draw_index(quadModel->GetIndexTotalCount(), 0u, 0u);
	}
}

void UIObjectRenderingInstance::AddUIObjectUniformBuffer()
{
	BufferLoadDesc uiObjectUniformBufferDesc = {};
	uiObjectUniformBufferDesc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	uiObjectUniformBufferDesc.m_desc.m_debugName = "UI Object Uniform Buffer";
	uiObjectUniformBufferDesc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	uiObjectUniformBufferDesc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	uiObjectUniformBufferDesc.m_rawData = nullptr;
	uiObjectUniformBufferDesc.m_size = sizeof(UIObjectUniformData);
	Buffer* object_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, uiObjectUniformBufferDesc);
	m_uiObjectUniformBufferList.push_back(object_uniform_buffer);

	m_uiObjectUniformDataList.push_back(UIObjectUniformData{});
}