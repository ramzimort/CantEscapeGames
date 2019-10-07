#pragma once

class EventManager;

class BaseEvent {
public:
	typedef std::size_t EventId;
	typedef std::unique_ptr<BaseEvent> Ptr;
	friend class EventManager;
public:
	virtual ~BaseEvent();
	float m_delay;
protected:
	BaseEvent();
protected:
	static EventId m_eventId;	 
};

