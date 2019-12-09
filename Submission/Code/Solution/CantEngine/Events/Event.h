
#pragma once

#include "BaseEvent.h"
#include "Multicast.h"

/**
 * @file Event.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief CRTP Derived Event Class
 */

class EventBus;
class InputManager;

/**
 * @brief CRTP Pattern for Event Inheritance
 * 
 * @tparam Derived 
 */
template <class Derived>
class Event : public BaseEvent 
{
public:
	typedef std::unique_ptr<Event> Ptr;
	virtual ~Event() = default;
	/**
	 * @brief Virtual function for mirror multicast invoke when event is invoked
	 * This is useful with some events for script delegate binds
	 * 
	 */
	virtual void operator()() = 0;
protected:
	Event() = default;

private:
	friend EventBus;
	friend InputManager;

	static EventId GetId()
	{
		static EventId refId = m_eventId++;
		return refId;
	}
};