/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "CameraManager.h"
#include "Graphics/Camera.h"


CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
	for (auto& node : m_registeredCameras) 
	{
		CameraInfo& camInfo = node.second;
		if (camInfo.m_camera != nullptr)
			delete camInfo.m_camera;
	}
	m_registeredCameras.clear();
}

void CameraManager::Update(float dt)
{
}

void CameraManager::RegisterCamera(const std::string& cameraTag, Camera* camera)
{
	CameraInfo cameraInfo(camera, 0, 0);
	m_registeredCameras[cameraTag] = cameraInfo;
}

void CameraManager::UnregisterCamera(const std::string& cameraTag)
{
}

void CameraManager::UnregisterCamera(unsigned cameraId)
{
}

const CameraInfo* CameraManager::GetCameraInfo(const std::string& nameTag) const
{
	auto find = m_registeredCameras.find(nameTag);
	if (find == m_registeredCameras.end())
	{
		return nullptr;
	}
	return &find->second;
}