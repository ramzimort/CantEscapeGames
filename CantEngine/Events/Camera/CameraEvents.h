#pragma once
#include "Events/Event.h"

/**
 * @file CameraEvents.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Camera Events
 */


class Camera;

/**
 * @brief Broadcasted when a camera gets destroyed
 * 
 */
class CameraDestructionEvent : public Event<CameraDestructionEvent>
{
public:
	/**
	 * @brief Broadcasted when a camera gets destroyed
	 * 
	 * @param camera 
	 * @param id 
	 */
	CameraDestructionEvent(Camera* camera, size_t id) :
		m_pCamera(camera),
		m_id(id)
	{
		DEBUG_LOG("Removing Camera: %d\n", id);
	}
	virtual ~CameraDestructionEvent() { }
	virtual void operator()() override
	{

	}
    /**
     * @brief Camera Pointer
     * 
     */
	Camera* m_pCamera;
    /**
     * @brief Camera ID
     * 
     */
	size_t m_id;
};
/**
 * @brief Broadcasted when a camera gets created for registration
 * 
 */
class CameraRegistrationEvent : public Event<CameraRegistrationEvent>
{
public:
	/**
	 * @brief Broadcasted when a camera gets created for registration
	 * 
	 * @param rCamera 
	 * @param xPos 
	 * @param yPos 
	 */
	CameraRegistrationEvent(Camera& rCamera, size_t xPos, size_t yPos) :
		m_camera(rCamera),
		m_xViewPortPos(xPos),
		m_yViewportPos(yPos)
	{
		DEBUG_LOG("Registering Camera: %p\n", &rCamera);
	}
	virtual ~CameraRegistrationEvent() { }
	virtual void operator()() override
	{

	}

    /**
     * @brief Camera Reference
     * 
     */
	Camera& m_camera;
    /**
     * @brief Position in Viewport x
     * 
     */
	size_t m_xViewPortPos;
    /**
     * @brief Position in Viewport y
     * 
     */
	size_t m_yViewportPos;
};