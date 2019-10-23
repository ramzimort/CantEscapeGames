


#include "Events/Event.h"

class SwitchStateEvent : public Event<SwitchStateEvent>
{
public:
	SwitchStateEvent(const std::string& pathName) :
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