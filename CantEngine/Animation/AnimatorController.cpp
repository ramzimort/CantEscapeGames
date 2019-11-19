///HEADER STUFF

#include "AnimatorController.h"


AnimatorController::AnimatorController() :
	dirtyFlag(0)
{
}

AnimatorController::~AnimatorController()
{
	for (auto& node : states) 
		delete node.second;
	states.clear();
}

AnimState *AnimatorController::GetCurrentState() const
{
	return current;
}

void AnimatorController::EnterState(AnimState *state)
{
	if (state == nullptr)
		return;

	this->current = state;
	this->current->isAnimRunning = true;
	this->current->animTime = 0.0f;
}


void AnimatorController::ExitState()
{
	this->current = nullptr;
}

void AnimatorController::SetEntryState(AnimState *entry)
{
	if (entry) 
	{
		this->entry = entry;

		//For now
		EnterState(entry);
	}
}

bool AnimatorController::IsStateNameValid(std::string const& name) 
{
	auto iter = states.find(name);
	if (iter == states.end())
		return true;
	return false;
}


AnimState *AnimatorController::GetState(std::string const& name) const
{
	auto iter = states.find(name);
	if (iter != states.end())
	{
		return iter->second;
	}
	return nullptr;
}

void AnimatorController::AddState(AnimState *newState)
{
	//First state added will be entry state (unless someone 
	//overwrites it by calling setEntry from script)
	if (entry == nullptr)
		entry = newState;

	states[newState->stateName] = newState;
}


void AnimatorController::SetTrigger(std::string const& trigger)
{
	//Modify trigger
	this->triggers[trigger] = 1;

	//Set dirty flag
	dirtyFlag = true;
}


void AnimatorController::ResetTriggers()
{
	//Reset all triggers to zero
	for (auto& node : triggers) 
		node.second = 0;
}