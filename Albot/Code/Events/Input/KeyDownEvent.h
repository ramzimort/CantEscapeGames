#pragma once

#include "Events/BaseEvent.h"
#include "Events/Event.h"

class KeyDownEvent : public Event<KeyDownEvent>
{
public:
	KeyDownEvent(SDL_Scancode key_scancode);
	~KeyDownEvent();

	SDL_Scancode m_scancode;
};