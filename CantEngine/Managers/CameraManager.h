#pragma once

/**
 * @file CameraManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Registers any created camera's data
 */

class Camera;
class CameraRegistrationEvent;
class CameraDestructionEvent;
class WindowSizeEvent;

/**
 * @brief Camera Information
 * 
 */
struct CameraInfo 
{
	/**
	 * @brief Camera Object Reference
	 * 
	 */
	Camera& m_camera;

	/**
	 * @brief Screen Left Bottom x-value
	 * 
	 */
	size_t m_leftBottomX;
	/**
	 * @brief Screen Right Bottom y-value
	 * 
	 */
	size_t m_leftBottomY;

	/**
	 * @brief Z order
	 * 
	 */
	int m_zOrder;
	/**
	 * @brief Construct a new Camera Info object
	 * 
	 * @param cam 
	 * @param x 
	 * @param y 
	 */
	CameraInfo(Camera& cam, size_t x, size_t y) :
		m_camera(cam), m_leftBottomX(x), 
		m_leftBottomY(y), m_zOrder(0)
	{}
};

/**
 * @brief Registers any created camera's data
 * This is important for any screen size or info change, the event must be passed to all cameras to update aspect ratios...
 * 
 */
class CameraManager
{
	typedef std::list<CameraInfo> Cameras;
public:
	CameraManager();
	~CameraManager();
	/**
	 * @brief Not used
	 * 
	 * @param dt 
	 */
	void Update(float dt);
	/**
	 * @brief Get the Main Camera object (index 0)
	 * 
	 * @return const CameraInfo& 
	 */
	const CameraInfo& GetMainCamera() const;

private:
	//Callbacks
	void OnCameraRegistration(const CameraRegistrationEvent* event);
	void OnCameraDestruction(const CameraDestructionEvent* event);
	void OnWindowSize(const WindowSizeEvent* event);

private:
	size_t m_scrWidth, m_scrHeight;
	Cameras m_cameras;
};

