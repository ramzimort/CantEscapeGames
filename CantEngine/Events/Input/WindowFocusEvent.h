#pragma once

#include "Events/Event.h"

class WindowFocusEvent : public Event<WindowFocusEvent>
{
public:
	WindowFocusEvent(bool gainedFocus) : 
		m_gainedFocus(gainedFocus) { }
	virtual ~WindowFocusEvent() { }

	bool m_gainedFocus;
};