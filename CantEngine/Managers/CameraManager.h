/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class Camera;
class CameraRegistrationEvent;
class CameraDestructionEvent;
class WindowSizeEvent;

//This is what the map will store. 
struct CameraInfo 
{
	//Camera should have the width height info
	Camera& m_camera;

	//This is in viewport coords
	size_t m_leftBottomX;
	size_t m_leftBottomY;

	//For now not used?
	int m_zOrder;

	CameraInfo(Camera& cam, size_t x, size_t y) :
		m_camera(cam), m_leftBottomX(x), 
		m_leftBottomY(y), m_zOrder(0)
	{}
};


class CameraManager
{
	typedef std::list<CameraInfo> Cameras;
public:
	CameraManager();
	~CameraManager();

	void Update(float dt);
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

