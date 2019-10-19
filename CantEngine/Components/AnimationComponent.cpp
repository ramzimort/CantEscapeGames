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


RTTR_REGISTRATION
{
	rttr::registration::class_<AnimationComponent>("AnimationComponent")
	.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
	.property("Clips", &AnimationComponent::m_clips);


	rttr::registration::class_<ClipInfo>("ClipInfo")
	.property("name", &ClipInfo::name)
	.property("path", &ClipInfo::path)
	.property("loops", &ClipInfo::loops);
}


AnimationComponent::AnimationComponent(GameObject *owner) :
	BaseComponent(owner, AnimationComponent::static_type)
{
	//CTOR
}


AnimationComponent::~AnimationComponent()
{
	//DTOR
}


void AnimationComponent::Init(ResourceManager* res, DXRenderer* dxrenderer) 
{
	//Once we have all the animations paths, we should load them and their data one by one
	std::string& name = this->m_clips[0].name;
	for (int i = 1; name != ""; ++i)
	{
		//Get path, load model
		std::string const& path = this->m_clips[i - 1].path;

		name = this->m_clips[i].name;
	}
}


void AnimationComponent::Begin(GameObjectManager *goMgr) 
{
	//At some point, decide if either this or the mesh comp will retrieve the per vertex info (index and weight)
	// TODO - Make sure m_animMap is complete
	// TODO - Make sure m_boneMap is complete


	//General setup of the animator
	this->m_currentAnimation = 0;

	//Get current animation from map
	auto iter = m_animationMap.find(m_clips[m_currentAnimation].name);
	if (iter != m_animationMap.end())
	{
		Animation const& anim = iter->second;
		this->m_currentTPS = anim.ticksPerSecond;
		this->m_duration = anim.duration;
		this->m_animationTime = 0.0f;
		this->m_loops = m_clips[m_currentAnimation].loops;
	}
}
