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
#include "Animation/AnimatorController.h"


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
	BaseComponent(owner, AnimationComponent::static_type)
{
	//Controller
	this->controller = new AnimatorController();
}


AnimationComponent::~AnimationComponent()
{
	//DTOR
	delete this->controller;
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
		for (int i = 0; name != ""; ++i)
		{
			//Get path, load model
			std::string const& path = this->m_clips[i].path;
			Assimp::Importer importer;
			aiScene const *scene = importer.ReadFile(path, 0);
			if (!scene || !scene->mRootNode)
			{
				name = this->m_clips[i + 1].name;
				continue;
			}

			//Load animation with FBX loader (Later change to res mgr)
			FBXLoader::LoadAnimationData(this->m_animationMap, scene, name);

			//Give animation info of wether it loops
			m_animationMap[name].loops = this->m_clips[i].loops;

			//setup animation event
			m_animationMap[name].EventsSetup();

			name = this->m_clips[i + 1].name;
		}
	}
}




//////////////////////////////////////////////////////
////              ANIMATOR CONTROLLER             ////
//////////////////////////////////////////////////////
void AnimationComponent::AnimationEnd(AnimState *anim)
{
	if (anim->animation->loops) 
	{
		anim->animTime = anim->animTime - anim->animation->duration;
	}
	else 
	{
		///OutputDebugString("Dirtyflag++ cause of animation end. -PrevVal: " + controller->dirtyFlag);  //////
		this->controller->dirtyFlag++;																  //////
		///OutputDebugString(" -NewVal: " + controller->dirtyFlag);									  //////
		///OutputDebugString("\n");																	  //////
		this->CheckForTransitionChanges(true);
		anim->isAnimRunning = false;
		///anim->animation->OnAnimationEnd();
	}
}

void AnimationComponent::FrameEndCleanUp()
{
	this->controller->ResetTriggers();

	this->controller->dirtyFlag = 0;
}


//EVERY TRANSITION CHECK WILL GO THROUGH HERE
//FOR CURRENT ANIMATION STATE, CHECK ALL ITS TRANSITIONS
void AnimationComponent::CheckForTransitionChanges(bool animEnd) 
{
	//If no dirty flag, no need to check
	if (controller->dirtyFlag > 0)
	{
		//Get current animator state (node of state machine)
		AnimState *current = controller->GetCurrentState();

		//If already in a transition, skip
		if (current->isInTransition)
			return;

		//Check all transitions against the controller's triggers
		current->CheckAllTransitions(controller->triggers, controller->dirtyFlag, animEnd);
	}
}

AnimState *AnimationComponent::CreateState(std::string stateName,
	std::string animName, float speed)
{
	AnimState *state = CreateState(stateName, animName);
	if (state) 
		state->speed = speed;
	return state;
}

AnimState *AnimationComponent::CreateState(std::string stateName, 
	std::string animName)
{
	if (controller->IsStateNameValid(stateName))
	{
		auto iter = m_animationMap.find(animName);
		if (iter != m_animationMap.end()) 
		{
			Animation& anim = iter->second;
			AnimState *state = new AnimState(stateName, &anim);
			state->speed = 1.0f;
			this->controller->AddState(state);
			return state;
		}
		else 
		{
			//TODO - anim not found
		}
	}
	return nullptr;
}

void AnimationComponent::SetEntryState(AnimState *entry)
{
	this->controller->SetEntryState(entry);
}

void AnimationComponent::SetTrigger(std::string const& trigger)
{
	controller->SetTrigger(trigger);
}

void AnimationComponent::AddAnimEvent(std::string const& animName, int tick, sol::table entry)
{
	auto iter = m_animationMap.find(animName);
	if (iter != m_animationMap.end()) 
	{
		Animation& anim = iter->second;
		anim.AddAnimEvent(tick, entry);
	}
}