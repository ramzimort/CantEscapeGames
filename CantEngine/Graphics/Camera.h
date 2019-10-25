/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

enum CameraProjectionType
{
	PROJECTION_PERSPECTIVE,
	//your normal orthographic camera
	PROJECTION_ORTHOGRAPHIC,
	//follows window size with viewport render information
	PROJECTION_ORTHOGRAPHIC_VIEWPORT,
};

class Camera
{
public:
	friend class FPSCameraSystem;
	friend class CameraComponent;
public:
	Camera();
	Camera(const Camera& lhs);

	Camera(float fov,
		float nearVal, float farVAl, const Vector3& position);
	~Camera();

	void SetAspectRatio(size_t width, size_t height);
	void SetProjectionType(CameraProjectionType projectionType);

	void SetCameraPosition(float x, float y, float z);
	void SetCameraPosition(const Vector3& new_position);

	Vector3 GetCameraPosition() const;
	const Vector3& GetForward() const;
	const Vector3& GetRight() const;

	void Reset();

	void ApplyRotation(const Matrix& transformation_mat);


	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix& GetInvViewMatrix() const { return m_invViewMatrix; }
	const Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
	const Matrix& GetInvProjectionMatrix() const { return m_invProjectionMatrix; }
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	const Matrix& GetInvViewProjectionMatrix() const { return m_invViewProjectionMatrix; }
	
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	size_t GetId();
	std::string GetCameraName() const;
	int32_t GetZOrder() const;

	const Vector4& GetViewportRenderInformation() const;

	size_t GetScreenWidth() const;
	size_t GetScreenHeight() const;


private:
	void InitFromComponent();
private:

	std::string m_cameraName;
	int32_t m_zOrder;
	//XY is  the viewport location in the window , ZW is the view port width & height 
	Vector4 m_viewportRenderInformation;

	CameraProjectionType m_cameraProjectionType;
	
	size_t m_id;	
	Vector3 m_position, m_lookDir, m_upDir, m_rightDir;

	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;

	Matrix m_invViewMatrix;
	Matrix m_invProjectionMatrix;

	Matrix m_viewProjectionMatrix;
	Matrix m_invViewProjectionMatrix;

	size_t m_width;
	size_t m_height;

	size_t m_orthographicWidth;
	size_t m_orthographicHeight;

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

	size_t screenWidth;
	size_t screenHeight;


	static size_t ID()
	{
		static size_t ID = 0;
		return ID++;
	}

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

