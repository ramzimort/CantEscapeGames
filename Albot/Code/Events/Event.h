
#pragma once

#include "BaseEvent.h"

class EventBus;

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
	static EventId GetId()
	{
		static EventId refId = m_eventId++;
		return m_eventId;
	}
};