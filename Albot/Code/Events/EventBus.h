#pragma once
#include "BaseEvent.h"
#include "Event.h"
#include "EventCallback.h"

class EventBus
{
	typedef std::vector<std::pair<BaseEvent::EventId, BaseEvent::Ptr>> EventQueue;
	typedef std::map<void*, BaseCallback::Ptr> BaseCallbackList;
	typedef std::unordered_map<BaseEvent::EventId, BaseCallbackList> CallbackMap;
public:
	EventBus();
	~EventBus();
	void Update(float dt);

	template <class T, typename ... Args>
	void QueueEvent(bool direct_call = false, Args&& ... args);
	template<class T> 
	void AddSubscriber(void* subscriber, const EventCallBack<T>& ptr);
	template<class T> 
	void DeleteSubscriber(void* objPtr);

private:
	EventQueue m_eventQueue;
	EventQueue m_eventBuffer;
	CallbackMap m_subscriberMap;
};

template <class T, typename ... Args>
void EventBus::QueueEvent(bool direct_call, Args&& ... event_args)
{
	std::unique_ptr<Event<T>> event(new T(std::forward<Args>(event_args) ...));
	std::size_t eventId = Event<T>::GetId();

	if (!direct_call)
	{
		m_eventBuffer.push_back(std::make_pair(eventId, std::move(event_args)));
		return;
	}
	auto it = m_subscriberMap.find(eventId);
	if (it != m_subscriberMap.end())
	{
		BaseCallbackList& cbList = it->second;
		for (auto& it = cbList.begin(); it != cbList.end(); ++it)
		{
			it->second->Call(event);
		}
	}
}

template<class T>
void EventBus::AddSubscriber(void* subscriber, const EventCallBack<T>& ptr)
{
	std::unique_ptr<EventCallBack<T>> eventCallBack(
		new EventCallBack<T>(
			subscriber,
			ptr));

	BaseEvent::EventId eventId = Event<T>::GetId();
	auto& it  = m_subscriberMap.find(eventId);

	if (it == m_subscriberMap.end())
		m_subscriberMap.insert(std::make_pair(eventId, BaseCallbackList()));
	
	m_subscriberMap[eventId].insert(subscriber, ptr);
}

template<class T>
void EventBus::DeleteSubscriber(void* objPtr)
{
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