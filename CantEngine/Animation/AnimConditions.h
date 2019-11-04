//HEADER

#pragma once

enum class ComparisonType
{
	EQUAL,
	GREATER_EQUAL,
	GREATER,
	LESS_EQUAL,
	LESS,
};

struct AnimCondition
{
	std::string trigger;

	AnimCondition(std::string trig) :
		trigger(trig)
	{}
};

/// struct AnimCondition
/// {
/// 	AnimCondition()
/// 	{}
/// };

/// struct TriggerCondition : public AnimCondition
/// {
/// 	std::string trigger;
/// 
/// 	TriggerCondition(std::string trig) :
/// 		trigger(trig)
/// 	{}
/// };

/// template <typename T>
/// struct NumCondition : public AnimCondition
/// {
/// 	T number;
/// 	T thresshold;
/// 	ComparisonType operation;
/// 
/// 	NumCondition(T &num, T val) :
/// 		number(num), thresshold(val)
/// 	{}
/// };

/// //This will be the ones to replace empty stuff
/// struct EndCondition : public AnimCondition
/// {
/// 	//For now, only with triggers
/// 	EndCondition()
/// 	{}
/// };