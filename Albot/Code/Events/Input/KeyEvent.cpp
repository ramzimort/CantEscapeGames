
#include "KeyEvent.h"

KeyEvent::KeyEvent(SDL_Scancode scancode, bool press) :
	m_scancode(scancode),
	m_press(press)
{ 
	DEBUG_LOG("Key: %s\n", SDL_GetKeyName(SDL_GetKeyFromScancode(m_scancode)));
}

KeyEvent::~KeyEvent()
{ }