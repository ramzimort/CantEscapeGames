#pragma once
#include "BaseSystem.h"

class FollowCurvesPathComponent;
class TransformComponent;

struct FollowCurvesPathCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	FollowCurvesPathComponent* m_followCurvesPathComp;
	//Ctor
	FollowCurvesPathCompNode(TransformComponent *transform,
		FollowCurvesPathComponent* followCurvesPathComp) : m_transform(transform),
		m_followCurvesPathComp(followCurvesPathComp)
	{}
};


class FollowCurvesPathSystem :
	public BaseSystem
{
public:
	FollowCurvesPathSystem();
	virtual ~FollowCurvesPathSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;
public:
	//To compare when using templates
	static unsigned int const static_type;
private:

};

