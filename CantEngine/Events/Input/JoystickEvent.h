#pragma once

#include "Events/Event.h"

class JoystickEvent : public Event<JoystickEvent>
{
public:
	JoystickEvent(SDL_JoystickID ID, bool connected) : m_ID(ID), m_connected(connected)
	{
		DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	
	virtual void operator()() override
	{

	}
	virtual ~JoystickEvent() { }

	SDL_JoystickID m_ID;
	bool m_connected;
};