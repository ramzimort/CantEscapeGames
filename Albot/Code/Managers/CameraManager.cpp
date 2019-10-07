/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the 
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#include "CameraManager.h"
#include "Graphics/Camera.h"
#include "EventManager.h"
#include "Events/Camera/CameraEvents.h"
#include "Events/Input/WindowSizeEvent.h"

CameraManager* gCameraManager;

CameraManager::CameraManager()
{
	gCameraManager = this;

	EventManager::Get()->SubscribeEvent<CameraRegistrationEvent>(this,
		std::bind(&CameraManager::OnCameraRegistration, this, std::placeholders::_1));

	EventManager::Get()->SubscribeEvent<CameraDestructionEvent>(this,
		std::bind(&CameraManager::OnCameraDestruction, this, std::placeholders::_1));

	EventManager::Get()->SubscribeEvent<WindowSizeEvent>(this,
		std::bind(&CameraManager::OnWindowSize, this, std::placeholders::_1));
}

CameraManager::~CameraManager()
{
	m_cameras.clear();
}

void CameraManager::Update(float dt)
{
}

const CameraInfo& CameraManager::GetMainCamera() const
{
	return *m_cameras.begin();
}

void CameraManager::OnCameraRegistration(const CameraRegistrationEvent* event)
{
		// Where does the camera info come from ?? 
		CameraInfo cameraInfo(event->m_camera, event->m_xViewPortPos, event->m_yViewportPos);
		cameraInfo.m_camera.SetAspectRatio(m_scrWidth, m_scrHeight);
		m_cameras.push_back(cameraInfo);
}

void CameraManager::OnCameraDestruction(const CameraDestructionEvent* event)
{
	for(auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if (it->m_camera.GetId() == event->m_id)
		{
			m_cameras.erase(it);
			return;
		}
	}
}

void CameraManager::OnWindowSize(const WindowSizeEvent* event)
{
	m_scrWidth = event->m_width;
	m_scrHeight = event->m_height;
}
