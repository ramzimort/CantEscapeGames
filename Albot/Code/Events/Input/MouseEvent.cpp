

#include "MouseEvent.h"

MouseEvent::MouseEvent(Vector2 pos, Vector2 delta,  uint8_t state) :
	m_position(pos),
	m_deltaPosition(delta),
	m_state(state)
{
}

MouseEvent::~MouseEvent()
{ }