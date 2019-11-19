#pragma once
#include "Events/Event.h"

/**
 * @brief Joystick connected/disconnected (ID is managed by input manager)
 * 
 */
class JoystickEvent : public Event<JoystickEvent>
{
public:
	/**
	 * @brief Joystick connected/disconnected (ID is managed by input manager)
	 * 
	 * @param ID 
	 * @param connected 
	 */
	JoystickEvent(uint32_t ID, bool connected) : m_ID(ID), m_connected(connected)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	
	virtual ~JoystickEvent() { }
	virtual void operator()() override { }
	/**
	 * @brief Joystick ID
	 * 
	 */
	const uint32_t m_ID;
	/**
	 * @brief Connected = true, Disconnected = false
	 * 
	 */
	const bool m_connected;
};

/**
 * @brief Joystick axis moved. Motion is normalized
 * 
 */
class JoystickMotionEvent : public Event<JoystickMotionEvent>
{
public:
	/**
	 * @brief Joystick axis moved. Motion is normalized
	 * 
	 * @param ID 
	 * @param motionBallId 
	 * @param motion 
	 */
	JoystickMotionEvent(uint32_t ID, uint32_t motionBallId, float motion) :
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

	static Multicast<void(const uint32_t, const uint32_t, const float)>& OnJoyMotionEvent()
	{
		static Multicast<void(const uint32_t, const uint32_t, const float)> m;
		return m;
	}
	/**
	 * @brief Joystick ID
	 * 
	 */
	const uint32_t m_ID;
	/**
	 * @brief Axis ID
	 * 
	 */
	const uint32_t m_motionBallId;
	/**
	 * @brief Axis Value
	 * 
	 */
	const float m_motion;
};

/**
 * @brief Joystick button pressed/released
 * 
 */
class JoystickButtonEvent : public Event<JoystickButtonEvent>
{
public:
	/**
	 * @brief Joystick button pressed/released
	 * 
	 * @param ID 
	 * @param joyButton 
	 * @param state 
	 */
	JoystickButtonEvent(uint32_t ID, uint32_t joyButton, bool state) :
		m_ID(ID), m_button(joyButton), m_state(state)
	{
		//DEBUG_LOG("Joystick: %d, Connected: %b\n", ID, connected);
	};
	virtual ~JoystickButtonEvent() { }
	virtual void operator()() override
	{
		OnJoystickButtonEvent()(m_ID, m_button, m_state);
		//DEBUG_LOG("Button: %d, %d\n", m_button, m_state);
	}

	static Multicast<void(const uint32_t, const uint32_t, const bool)>& OnJoystickButtonEvent()
	{
		static Multicast<void(const uint32_t, const uint32_t, const bool)> m;
		return m;
	}

	const uint32_t m_ID;
	const uint32_t m_button;
	const bool m_state;
};