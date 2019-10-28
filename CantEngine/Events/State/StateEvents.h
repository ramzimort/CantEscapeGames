#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

class ResourcesLoadedEvent : public Event<ResourcesLoadedEvent>
{
public:
	ResourcesLoadedEvent()
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~ResourcesLoadedEvent() { }
	virtual void operator()()
	{
		OnResourcesLoaded();
	}
	static Multicast<void()>& OnResourcesLoaded()
	{
		static Multicast<void()> m;
		return m;
	}
};

class QuitEvent : public Event<QuitEvent>
{
public:
	QuitEvent()
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~QuitEvent() { }
	virtual void operator()()
	{
		OnQuit();
	}
	static Multicast<void()>& OnQuit()
	{
		static Multicast<void()> m;
		return m;
	}
};

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