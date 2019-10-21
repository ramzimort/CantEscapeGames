/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "AnimationComponent.h"
unsigned const AnimationComponent::static_type = BaseComponent::numberOfTypes++;

#include "GameObjects/GameObject.h"
#include "Managers/GameObjectManager.h"
#include "Managers/ResourceManager.h"

#include "Components/MeshComponent.h"
#include "Animation/AnimModel.h"
#include "Animation/FBXLoader.h"


RTTR_REGISTRATION
{
	rttr::registration::class_<AnimationComponent>("AnimationComponent")
	.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
	.property("Clips", &AnimationComponent::m_clips)
	.property("StartingAnimation", &AnimationComponent::startingAnimation);


	rttr::registration::class_<ClipInfo>("ClipInfo")
	.property("name", &ClipInfo::name)
	.property("path", &ClipInfo::path)
	.property("loops", &ClipInfo::loops);
}


AnimationComponent::AnimationComponent(GameObject *owner) :
	BaseComponent(owner, AnimationComponent::static_type), 
	transitionDuration(0.0f), transitionTime(0.0f), inTransition(false)
{
	//CTOR
}


AnimationComponent::~AnimationComponent()
{
	//DTOR
}


void AnimationComponent::Init(ResourceManager* res, DXRenderer* dxrenderer) 
{
	this->m_resMgr = res;
}


void AnimationComponent::Begin(GameObjectManager *goMgr) 
{
	//Temporary measure: Get animModel directly from meshComp
	MeshComponent *meshcomp = this->GetOwner()->GetComponent<MeshComponent>();
	if (meshcomp) 
	{
		//Get the model
		AnimModel *model = static_cast<AnimModel*>(meshcomp->GetModel());

		//Once we have all the animations paths, we should load them 
		//and their data one by one
		std::string name = this->m_clips[0].name;
		for (int i = 1; name != ""; ++i)
		{
			//Get path, load model
			std::string const& path = this->m_clips[i - 1].path;
			Assimp::Importer importer;
			aiScene const *scene = importer.ReadFile(path, 0);
			if (!scene || !scene->mRootNode)
			{
				name = this->m_clips[i].name;
				continue;
			}

			//Load animation with FBX loader (Later change to res mgr)
			FBXLoader::LoadAnimationData(this->m_animationMap, scene, name);

			name = this->m_clips[i].name;
		}
	}

	//Set starting animation. TODO - This can be set from json
	SetCurrentAnimation(startingAnimation, 0.0f);
}


void AnimationComponent::SetCurrentAnimation(std::string const& animName, float timeElapsed)
{
	//Find index based on name
	for (int i = 0; i < m_animationMap.size(); ++i) 
	{
		if (animName == m_clips[i].name)
			m_currentAnimation = i;
	}

	//Get current animation from map
	auto iter = m_animationMap.find(m_clips[m_currentAnimation].name);
	if (iter != m_animationMap.end())
	{
		Animation const& anim = iter->second;
		this->m_currentTPS = anim.ticksPerSecond;
		this->m_duration = anim.duration;
		this->m_animationTime = timeElapsed;
		this->m_loops = m_clips[m_currentAnimation].loops;
	}
}


void AnimationComponent::SwitchAnimation(std::string const& animName, 
	float transDuration)
{
	//For now, only if not in transition
	if (inTransition)
		return;

	//Check if the animation exists
	auto iter = m_animationMap.find(animName);
	if (iter != m_animationMap.end())
	{
		nextAnimName = animName;
		inTransition = true;
		transitionTime = 0.0f;
		transitionDuration = transDuration;
	}
}


void AnimationComponent::OnTransitionEndSwitchAnimation(float timeElapsed) 
{
	//Call that effectively starts the second animation after transition
	SetCurrentAnimation(nextAnimName, timeElapsed);

	nextAnimName = "";
	inTransition = false;
	transitionDuration = 0.0f;
	transitionTime = 0.0f;
}
