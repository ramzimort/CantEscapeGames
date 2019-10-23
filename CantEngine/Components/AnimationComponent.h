/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"
#include "Animation/Animation.h"
#include "Animation/Bone.h"

#define MAX_ANIM_CLIPS		16		//Maximum numbers of animations per obj

class GameObjectManager;


struct ClipInfo 
{
	std::string name;
	std::string path;
	bool loops;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};


class AnimationComponent : public BaseComponent
{

//Friend classes and typedefs
public:
	friend class Factory;
	friend class AnimationSystem;

//METHODS
public:
	//ctor and dtor
	AnimationComponent(GameObject *owner);
	virtual ~AnimationComponent();

	virtual void Init(ResourceManager* res, DXRenderer* dxrenderer);
	virtual void Begin(GameObjectManager *goMgr);

	void SwitchAnimation(std::string const& animName, float transDuration);

private:
	void SetCurrentAnimation(std::string const& animName, 
		float timeElapsed = 0.0f);
	void OnTransitionEndSwitchAnimation(float timeElapsed);

public:
	//To compare when using templates
	static ComponentId const static_type;
	
private:
	std::string startingAnimation;
	ClipInfo m_clips[MAX_ANIM_CLIPS]; 
	
	//Bone and animation information
	std::unordered_map<std::string, Animation> m_animationMap;
	
	//Animation parameters 
	short m_currentAnimation;
	float m_animationTime;
	float m_duration;
	float m_currentTPS;
	bool m_loops;

	//Switching anim variables
	std::string nextAnimName;
	float transitionDuration; //In ticks
	float transitionTime; //how much has elapsed since transition started
	bool inTransition;


	//Hold the pointer to use it later
	ResourceManager* m_resMgr = nullptr;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};