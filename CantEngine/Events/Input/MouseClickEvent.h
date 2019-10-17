#pragma once

#pragma once

#include "Events/Event.h"

class MouseClickEvent : public Event<MouseClickEvent>
{
public:
	MouseClickEvent(Uint8 button, bool state) :
		m_button(button),
		m_state(state)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f, State: %d\n", pos.x, pos.y, delta.x, delta.y, state);
		OnMouseClick()(button, state);
	}
	virtual ~MouseClickEvent() {}

	static Multicast<void(Uint8, bool)>& OnMouseClick()
	{
		static Multicast<void(Uint8, bool)> m;
		return m;
	}

	Uint8 m_button;
	bool m_state;
};