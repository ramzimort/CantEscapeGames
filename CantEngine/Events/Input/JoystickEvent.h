#pragma once

#include "Events/Event.h"

class JoystickEvent : public Event<JoystickEvent>
{
public:
	JoystickEvent(SDL_JoystickID ID, bool connected) : m_ID(ID), m_connected(connected)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	
	virtual void operator()() override
	{

	}
	virtual ~JoystickEvent() { }

	uint32_t m_ID;
	bool m_connected;
};

class JoystickMotionEvent : public Event<JoystickMotionEvent>
{
public:
	JoystickMotionEvent(int32_t ID, uint8_t motionBallId, float motion) : 
		m_ID(ID), m_motionBallId(motionBallId), m_motion(motion)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	virtual ~JoystickMotionEvent() { }
	virtual void operator()() override
	{
		OnJoyMotionEvent()(m_ID, m_motionBallId, m_motion);
		//DEBUG_LOG("Axis: %d\n", m_motionBallId);
		//DEBUG_LOG("Motion: %f,\n", m_motion);
	}

	static Multicast<void(int32_t, uint8_t, float)>& OnJoyMotionEvent()
	{
		static Multicast<void(int32_t, uint8_t, float)> m;
		return m;
	}

	const int32_t m_ID;
	const uint8_t m_motionBallId;
	const float m_motion;
};

class JoystickHatEvent : public Event<JoystickHatEvent>
{
public:
	JoystickHatEvent(int32_t ID, uint32_t hat) :
		m_ID(ID), m_hat(hat)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	virtual ~JoystickHatEvent() { }
	virtual void operator()() override
	{
		OnJoystickHatEvent()(m_ID, m_hat);
		//DEBUG_LOG("Button: %d, %d\n", m_hat);
	}

	static Multicast<void(uint32_t, int32_t)>& OnJoystickHatEvent()
	{
		static Multicast<void(uint32_t, int32_t)> m;
		return m;
	}

	const uint32_t m_ID;
	const uint32_t m_hat;
}; 

class JoystickBallEvent : public Event<JoystickBallEvent>
{
public:
	JoystickBallEvent(int32_t ID, uint32_t ball) :
		m_ID(ID), m_ball(ball)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	virtual ~JoystickBallEvent() { }
	virtual void operator()() override { } 

	const uint32_t m_ID;
	const uint32_t m_ball;
};

class JoystickButtonEvent : public Event<JoystickButtonEvent>
{
public:
	JoystickButtonEvent(uint32_t ID, int32_t joyButton, bool state) :
		m_ID(ID), m_button(joyButton), m_state(state)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	virtual ~JoystickButtonEvent() { }
	virtual void operator()() override
	{
		OnJoystickButtonEvent()(m_ID, m_button, m_state);
		DEBUG_LOG("Button: %d, %d\n", m_button, m_state);
	}

	static Multicast<void(uint32_t, int32_t, bool)>& OnJoystickButtonEvent()
	{
		static Multicast<void(uint32_t, int32_t, bool)> m;
		return m;
	}

	const uint32_t m_ID;
	const uint32_t m_button;
	const bool m_state;
};