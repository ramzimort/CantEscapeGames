/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"

class Transform;
class RendererComponent;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct RenderingCompNode : BaseSystemCompNode
{
	Transform *n_transform;
	RendererComponent *n_renderer;

	//Ctor
	RenderingCompNode(Transform *transform, 
		RendererComponent *renderer) : n_transform(transform), 
		n_renderer(renderer) 
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

private:
	RenderingSystem(RenderingSystem const& rhs);

protected:
	//protected vars

public:
	//To compare when using templates
	static unsigned int const static_type;
};