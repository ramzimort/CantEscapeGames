#pragma once
#include "Events/Event.h"

class MouseMotionEvent : public Event<MouseMotionEvent>
{
public:
	MouseMotionEvent(const Vector2 pos, const Vector2 delta) :
		m_position(pos),
		m_deltaPosition(delta)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f\n", pos.x, pos.y, delta.x, delta.y);
	}
	virtual ~MouseMotionEvent() {}
	virtual void operator()() override
	{
		OnMouseMotion()(m_position, m_deltaPosition);
	}

	static Multicast<void(Vector2, Vector2)>& OnMouseMotion()
	{
		static Multicast<void(Vector2, Vector2)> m;
		return m;
	}

	Vector2 m_position;
	Vector2 m_deltaPosition;
};

class MouseScrollEvent : public Event<MouseScrollEvent>
{
public:
	MouseScrollEvent(Sint32 x, Sint32 y) :
		m_xScroll(x), m_yScroll(y)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f, State: %d\n", pos.x, pos.y, delta.x, delta.y, state);
	}
	virtual ~MouseScrollEvent() {}
	virtual void operator()() override
	{
		OnMouseScroll()(m_xScroll, m_yScroll);
	}
	static Multicast<void(Sint32, Sint32)>& OnMouseScroll()
	{
		static Multicast<void(Sint32, Sint32)> m;
		return m;
	}
	Sint32 m_xScroll;
	Sint32 m_yScroll;
};

class MouseClickEvent : public Event<MouseClickEvent>
{
public:
	MouseClickEvent(Uint8 button, bool state) :
		m_button(button),
		m_state(state)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f, State: %d\n", pos.x, pos.y, delta.x, delta.y, state);
	}
	virtual ~MouseClickEvent() {}
	virtual void operator()() override
	{
		OnMouseClick()(m_button, m_state);
	}

	static Multicast<void(Uint8, bool)>& OnMouseClick()
	{
		static Multicast<void(Uint8, bool)> m;
		return m;
	}

	Uint8 m_button;
	bool m_state;
};