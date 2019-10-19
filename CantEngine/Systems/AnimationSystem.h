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


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct AnimatorCompNode : BaseSystemCompNode
{
	AnimationComponent *n_animator;

	//Ctor
	AnimatorCompNode(AnimationComponent *animator) :
		n_animator(animator)
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
	Vector3 CalculateInterpPos(float AnimationTime, AnimChannel const& animChannel);
	MathUtil::Quaternion CalculateInterpRot(float AnimationTime, AnimChannel const& animChannel);
	Vector3 CalculateInterpScale(float AnimationTime, AnimChannel const& animChannel);

	void ProcessRecursiveTransformationFromRoot(AnimationComponent *animator, Bone& node, 
		Matrix const& parentTransf, std::vector<Matrix>& BoneTransformationsForShader);


	//Vector that, for now, will be used to pass info to shader as uniform
	std::vector<Matrix> BoneTransformations;


public:
	//To compare when using templates
	static unsigned int const static_type;
};