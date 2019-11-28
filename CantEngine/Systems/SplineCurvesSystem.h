#pragma once
#include "BaseSystem.h"

class TransformComponent;
class SplineCurvesComponent;
class AppRenderer;

struct SplineCurvesCompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	SplineCurvesComponent* m_splineCurvesComp;
	//Ctor
	SplineCurvesCompNode(TransformComponent *transform,
		SplineCurvesComponent* splineCurvesComp) : m_transform(transform),
		m_splineCurvesComp(splineCurvesComp)
	{}
};


class SplineCurvesSystem :
	public BaseSystem
{
public:
	SplineCurvesSystem();
	virtual ~SplineCurvesSystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void Update(float dt, BaseSystemCompNode *compNode) override;

	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;
	void RegisterAppRenderer(AppRenderer* appRenderer);
public:
	//To compare when using templates
	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;
};

