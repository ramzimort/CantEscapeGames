/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"
#include "Helper/Hash.h"

#define MAX_RENDERER_MATERIAL_COUNT 50

class Material;
class GameObject;
class GameObjectManager;

class RendererComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class RenderingSystem;
	friend class UISystem;
	friend struct GameObjectDesc;
//Public interface
public:
	RendererComponent(GameObject *owner);
	virtual ~RendererComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;

	
	void SetXTileFactor(float x_factor);
	void SetYTileFactor(float y_factor);
public:
	//Unique class identifier
	static ComponentId const static_type;
private:
	//this is our main/default material
	StringId m_materialId;
	Material* m_pMaterial;

	StringId m_meshInstancesMaterialIDList[MAX_RENDERER_MATERIAL_COUNT] = {StringId("")};
	Material* m_pMeshInstancesMaterialList[MAX_RENDERER_MATERIAL_COUNT] = {NULL};
	bool m_overrideLoadedModelMaterial;

	float m_xTileFactor;
	float m_yTileFactor;

#ifdef DEVELOPER	
	friend class CantDebug::DebugManager;
#endif

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};