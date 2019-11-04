///HEADER STUFF

#pragma once

///INCLUDES
#include "Quaternion.h"
#include "Events/Multicast.h"


struct PosKey
{
	Vector3 position;
	float time;
};

struct RotKey
{
	MathUtil::Quaternion quaternion;
	float time;
};

struct ScaKey
{
	Vector3 scale;
	float time;
};


struct AnimChannel
{
	std::string boneName;
	std::vector<PosKey> PositionKeys;
	std::vector<RotKey> RotationKeys;
	std::vector<ScaKey> ScalingKeys;
};


struct Animation
{
	typedef Multicast<void(void)> AnimMulticast;

	std::string animName;
	float duration;
	float ticksPerSecond;
	bool loops;

	//Animation events (TBD)
	std::vector<AnimMulticast> animationEvents;

	//Animation end multicast
	AnimMulticast OnAnimationEnd;

	std::vector<AnimChannel> channels;
	std::unordered_map<std::string, AnimChannel> boneChannelMap;


	//METHODS
	void EventsSetup()
	{
		this->animationEvents.resize(static_cast<int>(this->duration) + 1);
	}

	//Lua stuff
	void AddAnimEvent(int tick, sol::table entry)
	{
		if (tick >= 0 && tick <= static_cast<int>(this->duration))
			animationEvents[tick].BindLuaFunction(entry);
	}

	void CallEvent(float animTime) 
	{
		if (animTime >= 0 && animTime <= static_cast<int>(this->duration)) 
		{
			animationEvents[static_cast<int>(animTime)]();
		}
	}
};