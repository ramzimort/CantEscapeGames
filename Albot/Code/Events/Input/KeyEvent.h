#pragma once

#include "Events/BaseEvent.h"
#include "Events/Event.h"

class KeyEvent : public Event<KeyEvent>
{
public:
	KeyEvent(SDL_Scancode key_scancode, bool Press = true);
	virtual ~KeyEvent();

	SDL_Scancode m_scancode;
	bool m_press;
};