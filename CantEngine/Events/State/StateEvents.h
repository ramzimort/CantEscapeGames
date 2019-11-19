#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

/**
 * @file StateEvents.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief State Events
 */

/**
 * @brief Request state get loaded in the background. State is stored in temporary load buffer
 * 
 */
class LoadStateEvent : public Event<LoadStateEvent>
{
public:
	/**
	 * @brief Request state get loaded in the background. State is stored in temporary load buffer
	 * 
	 * @param levelPath 
	 */
	LoadStateEvent(const std::string& levelPath) : m_levelPath(levelPath)
	{	DEBUG_LOG("Loading State: %s\n", levelPath.c_str()); }
	virtual ~LoadStateEvent() { }
	virtual void operator()() { };
	/**
	 * @brief Requested level's JSON Path
	 * 
	 */
	const std::string m_levelPath;
};

/**
 * @brief Pushes loaded state if there is one
 * 
 */
class PushLoadedStateEvent : public Event<PushLoadedStateEvent>
{

public:
	/**
	 * @brief Pushes loaded state if there is one
	 * 
	 */
	PushLoadedStateEvent()
	{	DEBUG_LOG("Pushing Loaded State!\n"); }
	virtual ~PushLoadedStateEvent() { }
	virtual void operator()() { };
};

/**
 * @brief State Manage broadcasts this when a state is finished loading
 * 
 */
class ResourcesLoadedEvent : public Event<ResourcesLoadedEvent>
{
public:
	/**
	 * @brief State Manage broadcasts this when a state is finished loading
	 * 
	 * @param fileName 
	 */
	ResourcesLoadedEvent(const std::string& fileName) : m_fileName(fileName) {	}
	virtual ~ResourcesLoadedEvent() { }
	virtual void operator()() override	{	OnResourcesLoaded()(m_fileName); }
	static Multicast<void(const std::string)>& OnResourcesLoaded()
	{
		static Multicast<void(const std::string)> m;
		return m;
	}
	/**
	 * @brief Name of state with resources loaded
	 * 
	 */
	const std::string m_fileName;
};

/**
 * @brief Broadcast to safely shut down the engine
 * 
 */
class QuitEvent : public Event<QuitEvent>
{
public:
	/**
	 * @brief Broadcast to safely shut down the engine
	 * 
	 */
	QuitEvent()	{ }
	virtual ~QuitEvent() { }
	virtual void operator()() {}
};

/**
 * @brief Pushes a new state
 * 
 */
class PushStateEvent : public Event<PushStateEvent>
{
public:
	/**
	 * @brief Pushes a new state
	 * 
	 * @param levelPath 
	 */
	PushStateEvent(const std::string& levelPath) :
		m_path(levelPath) { }
	virtual ~PushStateEvent() { }
	void operator()() override {}
	/**
	 * @brief Requested level's JSON Path
	 * 
	 */
	const std::string m_path;
};

/**
 * @brief Pops current state
 * 
 */
class PopStateEvent : public Event<PopStateEvent>
{
public:
	/**
	 * @brief Pops current state
	 * 
	 */
	PopStateEvent()	{ }
	virtual ~PopStateEvent() { }
	void operator()() override {}
};