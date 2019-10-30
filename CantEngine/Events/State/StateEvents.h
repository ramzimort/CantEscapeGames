#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

class LoadStateEvent : public Event<LoadStateEvent>
{

public:
	LoadStateEvent(const std::string& levelPath) : m_levelPath(levelPath)
	{
		DEBUG_LOG("Loading State: %s\n", levelPath.c_str());
	}
	virtual ~LoadStateEvent() { }
	virtual void operator()() { };

	const std::string m_levelPath;
};

class PushLoadedStateEvent : public Event<PushLoadedStateEvent>
{

public:
	PushLoadedStateEvent()
	{
		DEBUG_LOG("Pushing Loaded State!\n");
	}
	virtual ~PushLoadedStateEvent() { }
	virtual void operator()() { };
};


class ResourcesLoadedEvent : public Event<ResourcesLoadedEvent>
{
public:
	ResourcesLoadedEvent(const std::string& fileName) : m_fileName(fileName)
	{
		DEBUG_LOG("Resources Finished Loading!\n");
	}
	virtual ~ResourcesLoadedEvent() { }
	virtual void operator()() { }

	const std::string m_fileName;
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
	virtual void operator()() {}
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
	void operator()() override {}
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
		DEBUG_LOG("Popping State!\n");
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PopStateEvent() { }
	virtual void operator()() {}

	//static Multicast<void(int, bool)>& OnNewStatePushed()
	//{
	//	static Multicast<void(int, bool)> m;
	//	return m;
	//}

	//const std::string m_path;
};