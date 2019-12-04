/**
 * @file AnimatorController.h
 * @author Jose Rosenbluth
 * @brief AnimatorController holds many classes related to the animation state machine
 * @date 2019-11-21
 */

#pragma once

///INCLUDES
#include "Animation/Animation.h"
#include "Events/Multicast.h"
#include "Animation/AnimConditions.h"

struct AnimState;
struct AnimTransition;
struct AnimCondition;

/**
 * @brief Describes a animation transition. This will be the structures running when jumping from one animation state to the next
 * 
 */
struct AnimTransition 
{
	/**
	 * @brief Origin animation
	 * 
	 */
	AnimState *from;
	/**
	 * @brief Target animation
	 * 
	 */
	AnimState *to;
	/**
	 * @brief Duration of transition, in ticks
	 * 
	 */
	float transitionDuration;
	/**
	 * @brief Time the transition has been running
	 * 
	 */
	float transitionTime;

	/**
	 * @brief Vector of conditions. If empty, we can enter this transition freely
	 * 
	 */
	std::vector<AnimCondition> conditions;

	/**
	 * @brief Construct a new Anim Transition object
	 * 
	 * @param from 
	 * @param to 
	 * @param dur 
	 */
	AnimTransition(AnimState *from, AnimState *to, float dur) :
		from(from), to(to), transitionDuration(dur), transitionTime(0.0f)
	{}

	/**
	 * @brief Checks and "logical ands" the conditions. Returns true if every condition is true
	 * 
	 * @param triggers  
	 * @param dirtyFlag 
	 * @param animEnd 
	 * @return true 
	 * @return false 
	 */
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

/**
 * @brief Represents a node in the animation state machine
 * 
 */
struct AnimState
{
	/**
	 * @brief Name of the state
	 * 
	 */
	std::string stateName;
	/**
	 * @brief Name of the animation stores in the state
	 * 
	 */
	Animation *animation;
	/**
	 * @brief Animation time describes how much time the animation has been running (in ticks)
	 * 
	 */
	float animTime;
	/**
	 * @brief Speed of the animation (1.0 is default)
	 * 
	 */
	float speed;
	/**
	 * @brief Returns true if the animation is running. False if we are in transition
	 * 
	 */
	bool isAnimRunning;
	/**
	 * @brief Returns true if we are in a transition, true if the animation is running
	 * 
	 */
	bool isInTransition;
	/**
	 * @brief Returns the current transition (if we are in a transition, Null otherwise)
	 * 
	 */
	AnimTransition *currentTransition;
	/**
	 * @brief Vector of transitions that this animation state has
	 * 
	 */
	std::vector<AnimTransition*> transitions;


	/**
	 * @brief Construct a new Anim State object
	 * 
	 * @param name 
	 * @param anim 
	 */
	AnimState(std::string name, Animation *anim) : 
		stateName(name), animation(anim), 
		isInTransition(false), currentTransition(nullptr)
	{
		animTime = 0.0f;
		isAnimRunning = false;
	}

	/**
	 * @brief Destroy the Anim State object
	 * 
	 */
	~AnimState()
	{
		for (AnimTransition *transition : transitions)
			delete transition;
		transitions.clear();
	}

	/**
	 * @brief Adds a new transition to this state. Used in setting up state from script
	 * 
	 * @param target 
	 * @param duration 
	 * @param triggers 
	 */
	void SetTransition(AnimState *target, float duration, 
		std::vector<std::string> triggers) 
	{
		AnimTransition *trans = new AnimTransition(this, target, duration);

		//We are creating a condition and pushing it
		for (auto trigger : triggers)
			trans->conditions.push_back(AnimCondition(trigger));
		
		this->transitions.push_back(trans);
	}

	/**
	 * @brief Check for all transitions and enters the first one it can
	 * 
	 * @param triggers 
	 * @param dirtyFlag 
	 * @param animEnd 
	 * @return true 
	 * @return false 
	 */
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

	/**
	 * @brief Called when entering a transition
	 * 
	 * @param transition 
	 */
	void EnterTransition(AnimTransition *transition) 
	{
		isInTransition = true;

		//Call the interrupt event
		this->animation->onAnimInterruptEvent();
		
		isAnimRunning = false;
		transition->transitionTime = 0.0f;
		currentTransition = transition;
	}
	
	/**
	 * @brief Called when exiting a transition
	 * 
	 */
	void ExitTransition()
	{
		isInTransition = false;
		currentTransition->transitionTime = 0.0f;
		currentTransition = nullptr;

	}
};


/**
 * @brief Class that represents the animation state machine itself
 * 
 */
class AnimatorController
{
public:
	friend class AnimationComponent;
	friend class AnimationSystem;

public:
	/**
	 * @brief Construct a new Animator Controller object
	 * 
	 */
	AnimatorController();
	/**
	 * @brief Destroy the Animator Controller object
	 * 
	 */
	~AnimatorController();

	/**
	 * @brief Get the Current State on the state machine
	 * 
	 * @return AnimState* 
	 */
	AnimState *GetCurrentState() const;
	/**
	 * @brief Get a state of name equal to parameter name
	 * 
	 * @param name 
	 * @return AnimState* 
	 */
	AnimState *GetState(std::string const& name) const;

	/**
	 * @brief Returns true if the name for the state is valid
	 * 
	 * @param name 
	 * @return true 
	 * @return false 
	 */
	bool IsStateNameValid(std::string const& name);
	/**
	 * @brief Adds a new state to the state machine. Used in setup from script
	 * 
	 * @param newState 
	 */
	void AddState(AnimState *newState);
	/**
	 * @brief Set the Entry State for the animation state machine (this will be the first state to be entered when it runs)
	 * 
	 * @param entry 
	 */
	void SetEntryState(AnimState *entry);

	/**
	 * @brief Called when entering a state
	 * 
	 * @param state 
	 */
	void EnterState(AnimState *state);
	/**
	 * @brief Called when exiting a state
	 * 
	 */
	void ExitState();

	/**
	 * @brief Resets all trigger values to zero
	 * 
	 */
	void ResetTriggers();
	/**
	 * @brief Makes the dirty flag match the trigger count
	 *
	 */
	void CleanDirtyFlag();
	/**
	 * @brief Sets a trigger object to one
	 * 
	 * @param trigger 
	 */
	void SetTrigger(std::string const& trigger);
	void ForceResetTriggersAndFlag();

private:
	/**
	 * @brief Entry state for the animation state machine
	 * 
	 */
	AnimState *entry;
	/**
	 * @brief Current state of the animation state machine
	 * 
	 */
	AnimState *current;
	/**
	 * @brief Integer that represents how many triggers are left still on, and need to be checked to be back in zero
	 * 
	 */
	int dirtyFlag;

	/**
	 * @brief Hash map for the states of the state machine
	 * 
	 */
	std::unordered_map<std::string, AnimState*> states;

	/**
	 * @brief Map of triggers, used to check if the conditions for jumping states are true
	 * 
	 */
	std::unordered_map<std::string, int> triggers;
	///std::unordered_map<std::string, float&> floats;
	///std::unordered_map<std::string, int&> ints;
	///std::unordered_map<std::string, bool&> bools;
};