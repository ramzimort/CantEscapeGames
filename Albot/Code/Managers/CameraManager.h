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

	CameraInfo()
		:m_camera(nullptr),
		m_leftBottomX(0),
		m_leftBottomY(0),
		m_zOrder(0)
	{
	}

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
	void RegisterCamera(const std::string& cameraTag, Camera* camera);
	void UnregisterCamera(const std::string& cameraTag);
	void UnregisterCamera(unsigned cameraId);

	const CameraInfo* GetCameraInfo(const std::string& nameTag) const;

private:
	std::unordered_map<std::string, CameraInfo> m_registeredCameras;
};

