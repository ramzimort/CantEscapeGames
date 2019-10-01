#include "EventBus.h"
#include "Event.h"
#include "BaseEvent.h"

EventBus::EventBus()
{
}

EventBus::~EventBus()
{
}

void EventBus::Update(float dt)
{
	m_eventQueue = std::move(m_eventBuffer);

	for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
	{
		// Update event timers
		auto& pEvent = it->second;
		if (pEvent->m_delay > 0.f)
		{
			pEvent->m_delay -= dt;
			continue;
		}

		// Callback all subscribers
		BaseEvent::EventId eventId = it->first;
		auto it2 = m_subscriberMap.find(eventId);
		if (it2 != m_subscriberMap.end())
		{
			auto& cbList = it2->second;
			for (auto &cb : cbList)
			{
				cb->Call(pEvent.get());
			}
		}

		// Maybe not a good idea to erase and mark for deletion (Use intermediate buffer)
		m_eventQueue.erase(it);
	}
}