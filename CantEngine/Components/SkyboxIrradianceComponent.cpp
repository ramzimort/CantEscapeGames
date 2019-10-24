#include "stdafx.h"
#include "SkyboxIrradianceComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/GameObjectManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/DXResourceLoader.h"

unsigned const SkyboxIrradianceComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<SkyboxIrradianceComponent>("SkyboxIrradianceComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("SkyboxEquirectangularTextureID", &SkyboxIrradianceComponent::m_skyboxEquirectangularTextureID)
		.property("SkyboxEquirectangularIrradianceTextureID", &SkyboxIrradianceComponent::m_skyboxEquirectangularIrradianceTextureID)
		.method("Init", &SkyboxIrradianceComponent::Init);

}

SkyboxIrradianceComponent::SkyboxIrradianceComponent(GameObject *owner)
	:BaseComponent(owner, static_type),
	m_hasBakedData(false)
{
}


SkyboxIrradianceComponent::~SkyboxIrradianceComponent()
{
	SafeReleaseDelete(m_pSkyboxTexture);
	SafeReleaseDelete(m_pSkyboxIrradianceTexture);
	SafeReleaseDelete(m_pIblPrefilteredEnvMapTexture);
}


void SkyboxIrradianceComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	m_pSkyboxEquirectangularTexture = resMgr->GetTexture(m_skyboxEquirectangularTextureID);
	m_pSkyboxEquirectangularIrradianceTexture = resMgr->GetTexture(m_skyboxEquirectangularIrradianceTextureID);

	TextureDesc hdr_skybox_texture_desc = {};
	hdr_skybox_texture_desc.m_arraySize = 6u;
	hdr_skybox_texture_desc.m_miscFlags = Misc_Flags::MISC_TEXTURE_CUBE | Misc_Flags::MISC_GENERATE_MIPS;
	hdr_skybox_texture_desc.m_width = 1024;
	hdr_skybox_texture_desc.m_height = 1024;
	hdr_skybox_texture_desc.m_bindFlags = Bind_Flags::BIND_RENDER_TARGET | Bind_Flags::BIND_SHADER_RESOURCE | Bind_Flags::BIND_UNORDERED_ACCESS;
	hdr_skybox_texture_desc.m_usageType = Usage_Type::USAGE_DEFAULT;
	hdr_skybox_texture_desc.m_mipLevels = 6;
	hdr_skybox_texture_desc.m_imageFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	hdr_skybox_texture_desc.m_isSRGB = false;
	hdr_skybox_texture_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
	hdr_skybox_texture_desc.m_depth = 1;
	hdr_skybox_texture_desc.m_clearVal = ClearValue{ 0.f, 0.f, 0.f, 0.f };

	TextureLoadDesc load_hdr_skybox_texture_desc = {};
	load_hdr_skybox_texture_desc.m_generateMipMap = true;
	load_hdr_skybox_texture_desc.m_useDXLoader = false;
	load_hdr_skybox_texture_desc.m_tex_desc = &hdr_skybox_texture_desc;
	load_hdr_skybox_texture_desc.m_file_name = "";

	m_pSkyboxTexture = DXResourceLoader::Create_Texture(dxrenderer, load_hdr_skybox_texture_desc);

	hdr_skybox_texture_desc.m_width = 256;
	hdr_skybox_texture_desc.m_height = 256;

	m_pSkyboxIrradianceTexture = DXResourceLoader::Create_Texture(dxrenderer, load_hdr_skybox_texture_desc);

	hdr_skybox_texture_desc.m_width = 512;
	hdr_skybox_texture_desc.m_height = 512;
	hdr_skybox_texture_desc.m_mipLevels = 5;

	m_pIblPrefilteredEnvMapTexture = DXResourceLoader::Create_Texture(dxrenderer, load_hdr_skybox_texture_desc);
}

void SkyboxIrradianceComponent::Begin(GameObjectManager *goMgr)
{

}