#pragma once

/**
 * @file BaseEvent.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Base Event Class
 */

class EventManager;
/**
 * @brief Base Event Class that is mirrored for event callbacks
 * 
 */
class BaseEvent {
public:
	typedef std::size_t EventId;
	typedef std::unique_ptr<BaseEvent> Ptr;
	friend class EventManager;
public:
	virtual ~BaseEvent();
	/**
	 * @brief Virtual function for mirror multicast invoke when event is invoked
	 * This is useful with some events for script delegate binds
	 * 
	 */
	virtual void operator()() = 0;
	/**
	 * @brief Time Delay before executing event
	 * 
	 */
	float m_delay;
protected:
	BaseEvent();
protected:
	static EventId m_eventId;	 
};

