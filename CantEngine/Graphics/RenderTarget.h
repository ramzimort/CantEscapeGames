#pragma once
#include "Texture.h"
#include "Common_Renderer.h"

#include <array>


class RenderTarget;

enum LoadActionType
{
	DONT_CLEAR,
	CLEAR,
	LOAD_ACTION_COUNT
};

typedef struct LoadActionsDesc
{
	ClearValue     m_clear_color_values[MAX_RENDER_TARGET_ATTACHMENTS];
	LoadActionType m_load_actions_color[MAX_RENDER_TARGET_ATTACHMENTS];
	ClearValue     m_clear_depth_stencil;
	LoadActionType m_load_action_depth;
	LoadActionType m_load_action_stencil;
} LoadActionsDesc;


struct RenderTargetDesc
{
	TextureDesc m_texture_desc;
};

class RenderTarget
{
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	//RenderTarget(const RenderTargetDesc& desc);
	RenderTarget(const RenderTargetDesc& desc);
	~RenderTarget();

	void Init(RenderTargetDesc& desc);

	const RenderTargetDesc& get_desc() const;
	const ClearValue& get_clear_value() const;
	ID3D11RenderTargetView* get_rtv(uint32_t mip_slice);

	void Release();

	const Texture* get_texture() const;
	Texture* get_texture();
private:
	RenderTargetDesc m_desc;
	Texture* m_texture;
	union
	{
		//TODO: add support for more mip slice render target view
		ID3D11RenderTargetView** m_pp_rendertargetview;
		ID3D11DepthStencilView** m_pp_depth_stencil_view;
	};
};

