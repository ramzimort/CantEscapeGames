
#pragma once

#include "BaseEvent.h"
#include "Multicast.h"

class EventBus;
class InputManager;

template <class Derived>
class Event : public BaseEvent 
{
public:
	typedef std::unique_ptr<Event> Ptr;
	virtual ~Event() = default;
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