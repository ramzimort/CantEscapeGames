/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


// 1 - Include the headers and add the line for the static type
#include "AnimationSystem.h"
unsigned const AnimationSystem::static_type = BaseSystem::numberOfTypes++;

#include "../GameObjects/GameObject.h"
#include "Animation/Interpolation.h"
#include "Animation/AnimModel.h"

// 2 - Include the components you want to add
#include "Components/AnimationComponent.h"
#include "Components/MeshComponent.h"



AnimationSystem::AnimationSystem() :
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<AnimationComponent>();
	Push_required_comp<MeshComponent>();
}


void AnimationSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	AnimationComponent *animator = go->GetComponent<AnimationComponent>();
	MeshComponent *meshComp = go->GetComponent<MeshComponent>();

	AnimatorCompNode *component_node = new AnimatorCompNode(animator, meshComp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void AnimationSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	AnimatorCompNode *node = static_cast<AnimatorCompNode*>(compNode);
	AnimationComponent *animator = node->n_animator;
	MeshComponent *meshComp = node->n_meshComp;

	//UPDATE CODE GOES HERE------------------------------------------
	AnimModel *model = static_cast<AnimModel*>(meshComp->GetModel());

	//Cache some stuff
	std::string const& animName = animator->m_clips[animator->m_currentAnimation].name;

	//Different update depending on wether it is transitioning between animations
	if (animator->inTransition) 
	{
		float animTime = animator->m_animationTime;
		animator->transitionTime += dt * animator->m_currentTPS;

		//Get current and next animation from map
		auto iter_curr = animator->m_animationMap.find(animName);
		auto iter_next = animator->m_animationMap.find(animator->nextAnimName);
		if (iter_curr != animator->m_animationMap.end() && iter_next != animator->m_animationMap.end())
		{
			Animation const& currAnim = iter_curr->second;
			Animation const& nextAnim = iter_next->second;

			//Loop through the current anim channels, find the interp to next 
			//anim first frame, and get the VQS matrix
			for (AnimChannel const& animChannel : currAnim.channels)
			{
				std::string const& boneName = animChannel.boneName;
				auto nextAnimChannelIter = nextAnim.boneChannelMap.find(boneName);
				if (nextAnimChannelIter != nextAnim.boneChannelMap.end())
				{
					AnimChannel const& nextAnimChannel = nextAnimChannelIter->second;

					//Interp position and the others
					Vector3 interpPos = CalculateInterpPos(animTime, animator->transitionTime, 
						animator->transitionDuration, animChannel, nextAnimChannel);
					MathUtil::Quaternion interpQuat = CalculateInterpRot(animTime, animator->transitionTime, 
						animator->transitionDuration, animChannel, nextAnimChannel);
					Vector3 interpScale = CalculateInterpScale(animTime, animator->transitionTime, 
						animator->transitionDuration, animChannel, nextAnimChannel);

					//Build the VQS matrix for this bone
					Matrix vqs = MathUtil::GenVQSMatrix(interpPos, interpQuat, interpScale);

					//Pass the bone its VQS matrix
					Bone& b = model->boneMap.find(boneName)->second;
					b.vqs = vqs;
					b.updatedVQS = true;
				}
			}

			//After updating, update the matrices recursively from root down the hierarchy
			// TODO - Find a better way to reach the root
			// Replace bonemap with skeleton structure. And have a GetRoot function.
			Bone& root = model->boneMap["RootNode"];
			// TODO - In future, skeleton should call the recursive function maybe
			this->ProcessRecursiveTransformationFromRoot(meshComp, model, root, Matrix::Identity);

			//switch animation
			if (animator->transitionTime > animator->transitionDuration)
			{
				float nextAnimTimeElapsed = animator->transitionTime - animator->transitionDuration;
				animator->OnTransitionEndSwitchAnimation(nextAnimTimeElapsed);
			}
		}
	}
	else 
	{
		animator->m_animationTime += dt * animator->m_currentTPS;
		float animTime = animator->m_animationTime;

		//Get current animation from map
		auto iter = animator->m_animationMap.find(animName);
		if (iter != animator->m_animationMap.end())
		{
			Animation const& animation = iter->second;

			//Loop through the channels, find the interp, and get the VQS matrix
			for (AnimChannel const& animChannel : animation.channels)
			{
				std::string const& boneName = animChannel.boneName;

				//Interp position and the others
				Vector3 interpPos = CalculateInterpPos(animTime, animChannel);
				MathUtil::Quaternion interpQuat = CalculateInterpRot(animTime, animChannel);
				Vector3 interpScale = CalculateInterpScale(animTime, animChannel); //TODO - special scaling

				//Build the VQS matrix for this bone
				Matrix vqs = MathUtil::GenVQSMatrix(interpPos, interpQuat, interpScale);

				//Pass the bone its VQS matrix
				Bone& b = model->boneMap.find(boneName)->second;
				b.vqs = vqs;
				b.updatedVQS = true;
			}

			//After updating, update the matrices recursively from root down the hierarchy
			// TODO - Replace bonemap with skeleton structure. And have a GetRoot function.
			Bone& root = model->boneMap["RootNode"];
			// TODO - In future, skeleton should call the recursive function maybe
			this->ProcessRecursiveTransformationFromRoot(meshComp, model, root, Matrix::Identity);

			//Restart animation
			if (animTime > animator->m_duration && animator->m_loops)
			{
				animator->m_animationTime = animTime - animator->m_duration;
			}
		}
	}
}



void AnimationSystem::ProcessRecursiveTransformationFromRoot(MeshComponent *meshcomp, AnimModel *model,
	Bone& node, Matrix const& parentTransf)
{
	//If bone had VQS calculated, we use that instead of nodeTransformation
	if (node.updatedVQS)
	{
		node.updatedVQS = false;
		node.accumTransformation = node.vqs * parentTransf;
	}
	else
	{
		node.accumTransformation = node.nodeTransformation * parentTransf;
	}

	//Continue with children
	for (int i = 0; i < node.children.size(); ++i)
	{
		Bone& child = model->boneMap[node.children[i]];
		ProcessRecursiveTransformationFromRoot(meshcomp, model, child, node.accumTransformation);
	}

	//Update stuff using offset matrix
	Matrix result = node.offsetMatrix * node.accumTransformation;
	meshcomp->BoneTransformationsForShader[node.index] = result;
}



Vector3 AnimationSystem::CalculateInterpPos(float AnimationTime, AnimChannel const& animChannel)
{
	PosKey prevKey = animChannel.PositionKeys[0];
	for (PosKey const& posk : animChannel.PositionKeys)
	{
		//Return the key's position
		if (fabs(posk.time - AnimationTime) <= 0.000001f)
			return posk.position;

		//Return interp
		if (prevKey.time <= AnimationTime && posk.time >= AnimationTime)
		{
			float alpha = (AnimationTime - prevKey.time) / (posk.time - prevKey.time);
			return MathUtil::Lerp(prevKey.position, posk.position, alpha);
		}
		else
		{
			prevKey = posk;
		}
	}

	//If we exit the loop and do not find, 
	//just return last key for now
	return prevKey.position;
}



Vector3 AnimationSystem::CalculateInterpPos(float AnimationTime, float transitionTime,
	float transitionDuration, AnimChannel const& animChannel, AnimChannel const& nextAnimChannel)
{
	PosKey nextAnimKey = nextAnimChannel.PositionKeys[0];

	PosKey prevKey = animChannel.PositionKeys[0];
	for (PosKey const& posk : animChannel.PositionKeys)
	{
		//Return interp
		if (prevKey.time <= AnimationTime && posk.time >= AnimationTime)
		{
			float alpha = ((AnimationTime - prevKey.time) + transitionTime) / transitionDuration;
			return MathUtil::Lerp(prevKey.position, nextAnimKey.position, alpha);
		}
		else
		{
			prevKey = posk;
		}
	}

	//If loops exit, return next anim key
	return nextAnimKey.position;
}


MathUtil::Quaternion AnimationSystem::CalculateInterpRot(float AnimationTime, AnimChannel const& animChannel)
{

	RotKey prevKey = animChannel.RotationKeys[0];
	for (RotKey const& rotk : animChannel.RotationKeys)
	{
		//Return the key's position
		if (fabs(rotk.time - AnimationTime) <= 0.000001f)
			return rotk.quaternion;

		//Return interp
		if (prevKey.time <= AnimationTime && rotk.time >= AnimationTime)
		{
			float alpha = (AnimationTime - prevKey.time) / (rotk.time - prevKey.time);
			return MathUtil::Slerp(prevKey.quaternion, rotk.quaternion, alpha);
		}

		//Prepare for next iter
		else prevKey = rotk;
	}

	//If we exit the loop and do not find, 
	//just return last key for now
	return prevKey.quaternion;
}


MathUtil::Quaternion AnimationSystem::CalculateInterpRot(float AnimationTime, float transitionTime,
	float transitionDuration, AnimChannel const& animChannel, AnimChannel const& nextAnimChannel)
{
	RotKey nextAnimKey = nextAnimChannel.RotationKeys[0];

	RotKey prevKey = animChannel.RotationKeys[0];
	for (RotKey const& rotk : animChannel.RotationKeys)
	{
		//Return interp
		if (prevKey.time <= AnimationTime && rotk.time >= AnimationTime)
		{
			float alpha = ((AnimationTime - prevKey.time) + transitionTime) / transitionDuration;
			return MathUtil::Slerp(prevKey.quaternion, nextAnimKey.quaternion, alpha);
		}
		else 
		{
			prevKey = rotk;
		}
	}

	//If loops exit, return next anim key
	return nextAnimKey.quaternion;
}


Vector3 AnimationSystem::CalculateInterpScale(float AnimationTime, AnimChannel const& animChannel)
{
	ScaKey prevKey = animChannel.ScalingKeys[0];
	for (ScaKey const& scak : animChannel.ScalingKeys)
	{
		//Return the key's position
		if (fabs(scak.time - AnimationTime) <= 0.000001f)
			return scak.scale;

		//Return interp
		if (prevKey.time <= AnimationTime && scak.time >= AnimationTime)
		{
			float alpha = (AnimationTime - prevKey.time) / (scak.time - prevKey.time);
			return MathUtil::Lerp(prevKey.scale, scak.scale, alpha);
		}

		//Prepare for next iter
		else prevKey = scak;
	}

	//If we exit the loop and do not find, 
	//just return last key for now
	return prevKey.scale;
}


Vector3 AnimationSystem::CalculateInterpScale(float AnimationTime, float transitionTime,
	float transitionDuration, AnimChannel const& animChannel, AnimChannel const& nextAnimChannel)
{
	ScaKey nextAnimKey = nextAnimChannel.ScalingKeys[0];

	ScaKey prevKey = animChannel.ScalingKeys[0];
	for (ScaKey const& scak : animChannel.ScalingKeys)
	{
		//Return interp
		if (prevKey.time <= AnimationTime && scak.time >= AnimationTime)
		{
			float alpha = ((AnimationTime - prevKey.time) + transitionTime) / transitionDuration;
			return MathUtil::Lerp(prevKey.scale, nextAnimKey.scale, alpha);
		}
		else 
		{
			prevKey = scak;
		}
	}

	//If loops exit, return next anim key
	return nextAnimKey.scale;
}