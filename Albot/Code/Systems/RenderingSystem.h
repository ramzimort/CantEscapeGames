/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"

class TransformComponent;
class RendererComponent;
class MeshComponent;
class AppRenderer;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct RenderingCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	RendererComponent *m_renderer;
	MeshComponent* m_meshesComponent;

	//Ctor
	RenderingCompNode(TransformComponent *transform, 
		RendererComponent *renderer, MeshComponent* meshesComp) : m_transform(transform),
		m_renderer(renderer), m_meshesComponent(meshesComp)
	{}
};


class RenderingSystem : public BaseSystem 
{

//Friend classes
public:
	friend class Factory;
//Public interface
public:
	RenderingSystem();
	virtual ~RenderingSystem() {}

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;
	void RegisterAppRenderer(AppRenderer* appRenderer);

private:
	RenderingSystem(RenderingSystem const& rhs);
protected:
	//protected vars

public:
	//To compare when using templates
	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;
};