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
class AnimatorController;
struct AnimState;


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
	friend class ScriptingManager;
	friend class AnimationSystem;

//METHODS
public:
	//ctor and dtor
	AnimationComponent(GameObject *owner);
	virtual ~AnimationComponent();

	virtual void Init(ResourceManager* res, DXRenderer* dxrenderer);
	virtual void Begin(GameObjectManager *goMgr);

	///void SwitchAnimation(std::string const& animName, float transDuration);

	void AnimationEnd(AnimState *anim);
	
	//Animator methods
	void SetTrigger(std::string const& trigger); 
	AnimState *CreateState(std::string stateName, std::string animName,
		float speed); 
	AnimState *CreateState(std::string stateName, std::string animName);
	void SetEntryState(AnimState *entry);


private:
	///void SetCurrentAnimation(std::string const& animName, 
	///	float timeElapsed = 0.0f);
	///void OnTransitionEndSwitchAnimation(float timeElapsed); 

	//Called from the system every frame
	void FrameEndCleanUp(); 
	void CheckForTransitionChanges(bool animEnd = false);

	//Register animation events from the scripting
	void AddAnimEvent(std::string const& animName, int tick, sol::table entry);
	void AddAnimEndEvent(std::string const& animName, sol::table entry);
	void AddInterruptEvent(std::string const& animName, sol::table entry);

public:
	//To compare when using templates
	static ComponentId const static_type;
	
private:

	//AnimatorController
	AnimatorController *controller;

	std::string startingAnimation;
	ClipInfo m_clips[MAX_ANIM_CLIPS]; 
	
	//Bone and animation information
	std::unordered_map<std::string, Animation> m_animationMap;
	
	//Animation parameters 
	std::string m_currentAnimation;
	/// float m_animationTime;
	/// float m_duration;
	/// float m_currentTPS;
	/// bool m_loops;
	/// 
	/// //Switching anim variables
	/// std::string nextAnimName;
	/// float transitionDuration; //In ticks
	/// float transitionTime; //how much has elapsed since transition started
	/// bool inTransition;

	//Hold the pointer to use it later
	ResourceManager* m_resMgr = nullptr;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};