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

class Material;
class GameObject;

class RendererComponent : public BaseComponent
{

//Friend classes
public:
	friend class Factory;
	friend class RenderingSystem;
	friend struct GameObjectDesc;
//Public interface
public:
	RendererComponent(GameObject *owner);
	virtual ~RendererComponent();

	virtual void Init() override;
	virtual void Begin() override;

	//TEMP FUNCTION:
	//Added by albert, there should not be a public function to set material
	//TODO
	void SetMaterial(Material* material);
	
public:
	//Unique class identifier
	static ComponentId const static_type;

private:
	std::string m_material_name;
	Material* m_pMaterial;

	float m_x_tile_factor;
	float m_y_tile_factor;
};