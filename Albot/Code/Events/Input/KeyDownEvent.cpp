
#include "KeyDownEvent.h"

KeyDownEvent::KeyDownEvent(SDL_Scancode scancode) : 
	m_scancode(scancode)
{ 
	DEBUG_LOG("Key: %s", SDL_GetKeyFromScancode(m_scancode));
}

KeyDownEvent::~KeyDownEvent()
{ }