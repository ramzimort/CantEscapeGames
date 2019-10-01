
#pragma once

#include "BaseEvent.h"

template <class Derived>
class Event : public BaseEvent 
{
public:
	typedef std::unique_ptr<Event> Ptr;
	virtual ~Event() = default;
protected:
	Event() = default;
private:
	static EventId GetId() 
	{
		static EventId refId = m_eventId++;
		return m_eventId;
	}
};