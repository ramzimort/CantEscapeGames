///HEADER STUFF

#pragma once

///INCLUDES
#include "Quaternion.h"


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
	std::string animName;
	float duration;
	float ticksPerSecond;

	std::vector<AnimChannel> channels;
};