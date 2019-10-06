/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


#include "CameraManager.h"
#include "Graphics/Camera.h"
#include "EventManager.h"
#include "Events/Camera/CameraRegistrationEvent.h"

CameraManager* gCameraManager;


CameraManager::CameraManager()
{
	gCameraManager = this;

	EventManager::Get()->SubscribeEvent<CameraRegistrationEvent>(this,
		std::bind(&CameraManager::OnCameraRegistration, this, std::placeholders::_1));
}

CameraManager::~CameraManager()
{
	m_registeredCameras.clear();
}

void CameraManager::Update(float dt)
{
}

void CameraManager::UnregisterCamera(const std::string& cameraTag)
{
}

void CameraManager::UnregisterCamera(unsigned cameraId)
{
}

const CameraInfo& CameraManager::GetMainCamera() const
{
	return m_registeredCameras[0];
}

void CameraManager::OnCameraRegistration(const CameraRegistrationEvent* event)
{
	if (event->m_state)
	{
		CameraInfo cameraInfo(event->m_camera, m_scrWidth, m_scrHeight);
		cameraInfo.m_camera.SetAspectRatio(m_scrWidth, m_scrHeight);
	}
	else
	{

	}

}
