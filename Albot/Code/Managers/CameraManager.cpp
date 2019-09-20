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
	for (auto& node : RegisteredCameras) 
	{
		CameraInfo& camInfo = node.second;
		if (camInfo.camera != nullptr)
			delete camInfo.camera;
	}
	RegisteredCameras.clear();
}

void CameraManager::Update(float dt)
{
}

void CameraManager::RegisterCamera(Camera* camera)
{
}

void CameraManager::UnregisterCamera(std::string& cameraTag)
{
}

void CameraManager::UnregisterCamera(unsigned cameraId)
{
}