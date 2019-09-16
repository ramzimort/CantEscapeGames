/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>
#include <string>

class Camera;


//This is what the map will store. 
struct CameraInfo 
{
	//Camera should have the width height info
	Camera *camera;

	//This is in viewport coords
	int leftBottomX;
	int leftBottomY;

	//For now not used?
	int zOrder;

	CameraInfo(Camera* cam, int x, int y) : 
		camera(cam), leftBottomX(x), 
		leftBottomY(y), zOrder(0)
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
	std::unordered_map<std::string, CameraInfo&> RegisteredCameras;
};

