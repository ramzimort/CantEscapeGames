#pragma once

#include "Events/BaseEvent.h"
#include "Events/Event.h"

class MouseEvent : public Event<MouseEvent>
{
public:
	MouseEvent(Vector2 pos, Vector2 delta, uint8_t state);
	virtual ~MouseEvent();

	Vector2 m_position;
	Vector2 m_deltaPosition;
	uint8_t m_state;
};