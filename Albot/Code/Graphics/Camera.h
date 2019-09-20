/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

//Placeholder
struct Matrix4 {};


class Camera
{
public:
	Camera();
	Camera(int width, int height, float fov,
		float near, float far, Vector4& eye, 
		Vector4& look, bool isOrtho);
	~Camera();

	void Update(float dt);
	void GetView(Matrix4& view);
	void GetProj(Matrix4& proj);

	//Getters
	Vector4& GetEye() const;
	Vector4& GetLook() const;
	Vector2 GetSize() const;


private:
	void initCamera();
	void GetPerspective(Matrix4& proj);
	void GetOrthographic(Matrix4& proj);

private:
	int m_width, m_height;
	float m_fov, m_near, m_far, m_aspect;
	Vector4 m_look;
	Vector4 m_eye;
	bool m_isOrtho;

};

