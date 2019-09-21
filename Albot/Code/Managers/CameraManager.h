/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class Camera;


//This is what the map will store. 
struct CameraInfo 
{
	//Camera should have the width height info
	Camera *m_camera;

	//This is in viewport coords
	int m_leftBottomX;
	int m_leftBottomY;

	//For now not used?
	int m_zOrder;

	CameraInfo(Camera* cam, int x, int y) : 
		m_camera(cam), m_leftBottomX(x), 
		m_leftBottomY(y), m_zOrder(0)
	{}
};


class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void Update(float dt);
	void RegisterCamera(Camera* camera);
	void UnregisterCamera(std::string& cameraTag);
	void UnregisterCamera(unsigned cameraId);

private:
	std::unordered_map<std::string, CameraInfo&> m_registeredCameras;
};

