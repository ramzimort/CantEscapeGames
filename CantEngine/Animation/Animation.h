/**
 * @file Animation.h
 * @author Jose Rosenbluth
 * @brief Contains classes related to keyframe animation
 * @date 2019-11-21
 */

#pragma once

///INCLUDES
#include "Quaternion.h"
#include "Events/Multicast.h"

/**
 * @brief Describes a position keyframe
 * 
 */
struct PosKey
{
	/**
	 * @brief Vector describing position
	 * 
	 */
	Vector3 position;
	/**
	 * @brief Time in ticks
	 * 
	 */
	float time;
};

/**
 * @brief Describes a rotation keyframe
 * 
 */
struct RotKey
{
	/**
	 * @brief Quaternion describing orientation
	 * 
	 */
	MathUtil::Quaternion quaternion;
	/**
	 * @brief Time in ticks
	 * 
	 */
	float time;
};

/**
 * @brief Describes a scale keyframe
 * 
 */
struct ScaKey
{
	/**
	 * @brief Vector describing scale
	 * 
	 */
	Vector3 scale;
	/**
	 * @brief Time in ticks
	 * 
	 */
	float time;
};

/**
 * @brief Struct that holds all the animation keyframe info for a single bone
 * 
 */
struct AnimChannel
{
	/**
	 * @brief Bone that is being animated
	 * 
	 */
	std::string boneName;
	/**
	 * @brief All the position keys for this bone's animation
	 * 
	 */
	std::vector<PosKey> PositionKeys;
	/**
	 * @brief All the rotation keys for this bone's animation
	 * 
	 */
	std::vector<RotKey> RotationKeys;
	/**
	 * @brief All the scale keys for this bone's animation
	 * 
	 */
	std::vector<ScaKey> ScalingKeys;
};


/**
 * @brief Describes a keyframe animation for a skeleton
 * 
 */
struct Animation
{
	/**
	 * @brief Typedef used to define animation events (stored in a multicast form)
	 * 
	 */
	typedef Multicast<void(void)> AnimMulticast;

	/**
	 * @brief Animation name
	 * 
	 */
	std::string animName;
	/**
	 * @brief Duration in ticks
	 * 
	 */
	float duration;
	/**
	 * @brief Ticks per second (to transform between ticks and seconds or other units)
	 * 
	 */
	float ticksPerSecond;
	/**
	 * @brief If this animation loops or not
	 * 
	 */
	bool loops;

	/**
	 * @brief Vector of animation events, potentially one per keyframe
	 * 
	 */
	std::vector<AnimMulticast> animationEvents;

	/**
	 * @brief Vector of animation channels. Each channel has information about which bone is animated and its keyframe information
	 * 
	 */
	std::vector<AnimChannel> channels;
	/**
	 * @brief Hash table for bone channels
	 * 
	 */
	std::unordered_map<std::string, AnimChannel> boneChannelMap;

	/**
	 * @brief Setups the animation event's vector so it is of the right size
	 * 
	 */
	void EventsSetup()
	{
		this->animationEvents.resize(static_cast<int>(this->duration) + 1);
	}

	/**
	 * @brief Used for adding an animation event from LUA script
	 * 
	 * @param tick Which tick we want this event to happen
	 * @param entry Table which contains the lua self reference and the method it has to run
	 */
	void AddAnimEvent(int tick, sol::table entry)
	{
		if (tick >= 0 && tick <= static_cast<int>(this->duration))
			animationEvents[tick].BindLuaFunction(entry);
	}

	/**
	 * @brief Fires an animation event (if there is one)
	 * 
	 * @param animTime 
	 */
	void CallEvent(float animTime) 
	{
		if (animTime >= 0 && animTime <= static_cast<int>(this->duration)) 
		{
			animationEvents[static_cast<int>(animTime)]();
		}
	}
};