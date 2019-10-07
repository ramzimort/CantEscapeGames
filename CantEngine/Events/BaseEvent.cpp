
#include "BaseEvent.h"

BaseEvent::BaseEvent()
	:m_delay(-1.f)
{
}


BaseEvent::~BaseEvent()
{
}

BaseEvent::EventId BaseEvent::m_eventId;
