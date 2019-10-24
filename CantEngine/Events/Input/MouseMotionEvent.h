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