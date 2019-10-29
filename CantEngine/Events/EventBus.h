#pragma once

#include "BaseEvent.h"
#include "Event.h"
#include "EventCallback.h"
#include "Input/Input.h"

class EventBus
{
	typedef std::map<const void*, BaseCallback::Ptr> BaseCallbackList;
	typedef std::vector<std::pair<BaseEvent::EventId, BaseEvent::Ptr>> EventQueue;
	typedef std::unordered_map<BaseEvent::EventId, BaseCallbackList> CallbackMap;
public:
	EventBus();
	~EventBus();
	void Update(float dt);
	template <typename T, typename ... Args>
	void QueueEvent(bool direct_call = false, Args&& ... args);
	template<typename T> 
	void AddSubscriber(const EventCallback<T>& ptr);
	template<typename T>
	void DeleteSubscriber(void* objPtr);
private:
	EventQueue m_eventQueue;
	EventQueue m_eventBuffer;
	CallbackMap m_subscriberMap;
	std::mutex m_mutex;
};

template <typename T, typename ... Args>
void EventBus::QueueEvent(bool direct_call, Args&& ... event_args)
{
	std::unique_ptr<Event<T>> event(new T(std::forward<Args>(event_args) ...));
	std::size_t eventId = Event<T>::GetId();

	if (!direct_call)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_eventBuffer.push_back(std::make_pair(eventId, std::move(event)));
		return;
	}
	auto it = m_subscriberMap.find(eventId);
	if (it != m_subscriberMap.end())
	{
		BaseCallbackList& cbList = it->second;
		for (auto it = cbList.begin(); it != cbList.end(); ++it)
		{
			it->second->Call(event.get());
			(*event.get())();
		}
	}
}

template<typename T>
void EventBus::AddSubscriber(const EventCallback<T>& ptr)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::unique_ptr<EventCallback<T>> eventCallback(new EventCallback<T>(ptr));

	BaseEvent::EventId eventId = Event<T>::GetId();
	auto it  = m_subscriberMap.find(eventId);

	if (it == m_subscriberMap.end())
		m_subscriberMap.insert(std::make_pair(eventId, BaseCallbackList()));
	BaseCallbackList& cbList = m_subscriberMap[eventId];
	
	cbList.insert(std::make_pair(ptr.GetOwner(), std::move(eventCallback)));
}

template<typename T>
void EventBus::DeleteSubscriber(void* objPtr)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	BaseEvent::EventId eventId = Event<T>::GetId();
	auto iter = m_subscriberMap.find(eventId);
	if (iter != m_subscriberMap.end())
	{
		BaseCallbackList& cbList = iter->second;
		for (auto it = cbList.begin(); it != cbList.end(); ++it)
		{
			if (it->first == objPtr)
			{

				cbList.erase(it);
				break;
			}
		}
	}
}