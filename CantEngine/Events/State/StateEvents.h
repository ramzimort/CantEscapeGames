#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

class PushStateEvent : public Event<PushStateEvent>
{
public:
	PushStateEvent(const std::string& levelPath) :
		m_path(levelPath)
	{
		DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PushStateEvent() { }

	//static Multicast<void(int, bool)>& OnNewStatePushed()
	//{
	//	static Multicast<void(int, bool)> m;
	//	return m;
	//}

	const std::string m_path;
};

class PopStateEvent : public Event<PopStateEvent>
{
public:
	PopStateEvent()
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PopStateEvent() { }

	//static Multicast<void(int, bool)>& OnNewStatePushed()
	//{
	//	static Multicast<void(int, bool)> m;
	//	return m;
	//}

	//const std::string m_path;
};