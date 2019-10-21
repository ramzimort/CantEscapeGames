/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"
#include "Animation/Quaternion.h"
#include "Animation/Animation.h"
#include "Animation/Bone.h"

class AnimationComponent;
class MeshComponent;
class AnimModel;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct AnimatorCompNode : BaseSystemCompNode
{
	AnimationComponent *n_animator;
	MeshComponent *n_meshComp;

	//Ctor
	AnimatorCompNode(AnimationComponent *animator, MeshComponent *meshComp) :
		n_animator(animator), n_meshComp(meshComp)
	{}
};


class AnimationSystem : public BaseSystem
{
//Public interface
public:
	AnimationSystem();
	virtual ~AnimationSystem() {}

	virtual void Register_GameObject(GameObject *go);
	virtual void Update(float dt, BaseSystemCompNode *compNode);

private:
	//Interpolating keys when in a single animation
	Vector3 CalculateInterpPos(float AnimationTime, AnimChannel const& animChannel);
	MathUtil::Quaternion CalculateInterpRot(float AnimationTime, AnimChannel const& animChannel);
	Vector3 CalculateInterpScale(float AnimationTime, AnimChannel const& animChannel);

	//Interpolating keys when transitioning between animations
	Vector3 CalculateInterpPos(float AnimationTime, float transitionTime, float transitionDuration,
		AnimChannel const& animChannel, AnimChannel const& nextAnimChannel);
	MathUtil::Quaternion CalculateInterpRot(float AnimationTime, float transitionTime, float transitionDuration,
		AnimChannel const& animChannel, AnimChannel const& nextAnimChannel);
	Vector3 CalculateInterpScale(float AnimationTime, float transitionTime, float transitionDuration, 
		AnimChannel const& animChannel, AnimChannel const& nextAnimChannel);

	void ProcessRecursiveTransformationFromRoot(MeshComponent *meshcomp, AnimModel *model, Bone& node,
		Matrix const& parentTransf);


public:
	//To compare when using templates
	static unsigned int const static_type;
};