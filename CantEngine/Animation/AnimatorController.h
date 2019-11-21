///HEADER STUFF

#pragma once

///INCLUDES
#include "Animation/Animation.h"
#include "Events/Multicast.h"
#include "Animation/AnimConditions.h"

struct AnimState;
struct AnimTransition;
struct AnimCondition;

struct AnimTransition 
{
	AnimState *from;
	AnimState *to;
	float transitionDuration;
	float transitionTime;

	//Vector of conditions. If empty, we can enter this transition freely
	std::vector<AnimCondition> conditions;

	//METHODS
	AnimTransition(AnimState *from, AnimState *to, float dur) :
		from(from), to(to), transitionDuration(dur), transitionTime(0.0f)
	{}

	//Checks and "logical and"'s the conditions
	bool CheckCondition(std::unordered_map<std::string, int>& triggers,
		int& dirtyFlag, bool animEnd = false)
	{
		//If there are conditions, "logical and" them all
		if (conditions.size() > 0)
		{
			bool result = true;
			for (AnimCondition& condition : conditions)
			{
				int found = 0;
				auto iter = triggers.find(condition.trigger);
				if (iter != triggers.end())
				{
					found = iter->second;
					if (found)
					{
						triggers[condition.trigger] = 0;

						///OutputDebugString("Dirtyflag-- cause of using trigger. -PrevVal: " + dirtyFlag);  //////
						--dirtyFlag;	
						//int a = 132;
						///OutputDebugString(" -NewVal: " + dirtyFlag);									  //////
						///OutputDebugString("\n");														  //////
					}
				}
			
				result = result && found;
			}
			return result;
		}
		//If there are no conditions, then this is a animation ending. 
		//Return true, and decrease dirty flag
		else 
		{
			if (animEnd)
			{
				dirtyFlag--;
				return true;
			}
			else
				return false; //It was not an animEnd signal
		}
	}
};


struct AnimState
{
	std::string stateName;
	Animation *animation;
	float animTime;
	float speed;
	bool isAnimRunning;

	bool isInTransition;
	AnimTransition *currentTransition;

	std::vector<AnimTransition*> transitions;


	//METHODS
	AnimState(std::string name, Animation *anim) : 
		stateName(name), animation(anim), 
		isInTransition(false), currentTransition(nullptr)
	{
		animTime = 0.0f;
		isAnimRunning = false;
	}

	~AnimState()
	{
		for (AnimTransition *transition : transitions)
			delete transition;
		transitions.clear();
	}

	void SetTransition(AnimState *target, float duration, 
		std::vector<std::string> triggers) 
	{
		AnimTransition *trans = new AnimTransition(this, target, duration);

		//We are creating a condition and pushing it
		for (auto trigger : triggers)
			trans->conditions.push_back(AnimCondition(trigger));
		
		this->transitions.push_back(trans);
	}

	//Check for all transitions and enter the first one it can
	bool CheckAllTransitions(std::unordered_map<std::string, int>& triggers, 
		int& dirtyFlag, bool animEnd = false)
	{
		for (AnimTransition *transition : transitions)
		{
			//Check condition
			bool result = transition->CheckCondition(triggers, dirtyFlag, animEnd);
			if (result)
			{
				//Jump to that transition, stop this loop
				EnterTransition(transition);
				return true;
			}
		}

		return false;
	}

	//When entering a transition
	void EnterTransition(AnimTransition *transition) 
	{
		isInTransition = true;
		isAnimRunning = false;
		transition->transitionTime = 0.0f;
		currentTransition = transition;
	}
	
	//When entering a transition
	void ExitTransition()
	{
		isInTransition = false;
		currentTransition->transitionTime = 0.0f;
		currentTransition = nullptr;

	}
};



class AnimatorController
{
public:
	friend class AnimationComponent;
	friend class AnimationSystem;

public:
	AnimatorController();
	~AnimatorController();

	AnimState *GetCurrentState() const;
	AnimState *GetState(std::string const& name) const;

	bool IsStateNameValid(std::string const& name);
	void AddState(AnimState *newState);
	void SetEntryState(AnimState *entry);

	//These should called after and before transition changes
	void EnterState(AnimState *state);
	void ExitState();

	//INTERFACE
	void ResetTriggers();
	void SetTrigger(std::string const& trigger);

private:
	AnimState *entry;
	AnimState *current;
	int dirtyFlag;

	std::unordered_map<std::string, AnimState*> states;

	//PARAMETERS - (TODO, NOT YET)
	std::unordered_map<std::string, int> triggers;
	///std::unordered_map<std::string, float&> floats;
	///std::unordered_map<std::string, int&> ints;
	///std::unordered_map<std::string, bool&> bools;
};