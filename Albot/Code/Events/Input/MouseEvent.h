#pragma once

#include "Events/Event.h"

class MouseEvent : public Event<MouseEvent>
{
public:
	MouseEvent(Vector2 pos, Vector2 delta, uint8_t state) :	
		m_position(pos),
		m_deltaPosition(delta),
		m_state(state) { }
	virtual ~MouseEvent() { }

	Vector2 m_position;
	Vector2 m_deltaPosition;
	uint8_t m_state;
};