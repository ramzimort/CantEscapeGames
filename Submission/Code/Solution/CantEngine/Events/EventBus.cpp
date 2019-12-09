
#include "EventBus.h"

EventBus::EventBus()
{
}

EventBus::~EventBus()
{
}

void EventBus::Update(float dt)
{
	m_mutex.lock();
	m_eventQueue = std::move(m_eventBuffer);
	m_mutex.unlock();

	auto it = m_eventQueue.begin();
	while (it != m_eventQueue.end())
	{
		// Update event timers
		auto& pEvent = it->second;
		if (pEvent->m_delay > 0.f)
		{
			pEvent->m_delay -= dt;
			++it;
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
				cb.second->Call(pEvent.get());
			}
		}
		(*pEvent.get())();
		// Maybe not a good idea to erase and mark for deletion (Use intermediate buffer)
		it = m_eventQueue.erase(it);
	}
}