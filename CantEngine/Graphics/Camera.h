/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Albert Harley / Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once
/**
 * @file Camera.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public interface for 3D Camera
 */

enum CameraProjectionType
{
	PROJECTION_PERSPECTIVE,
	//your normal orthographic camera
	PROJECTION_ORTHOGRAPHIC,
	//follows window size with viewport render information
	PROJECTION_ORTHOGRAPHIC_VIEWPORT,
};

enum CameraRenderObjectType
{
	CAMERA_RENDER_UI = 1 << 0,
	CAMERA_RENDER_MESHES = 1 << 1,

	CAMERA_RENDER_UI_AND_MESHES = CAMERA_RENDER_UI | CAMERA_RENDER_MESHES
};

/**
 * @brief The public interface for 3D Camera
 * 
 */
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
	/**
	 * @brief Set the field of view (degree)
	 * 
	 * @param fov 
	 */
	void SetFOV(float fov);
	/**
	 * @brief Increase the field of view (degree)
	 * 
	 * @param fov 
	 */
	void IncreaseFOV(float fov);
	/**
	 * @brief Decrease the field of view (degree)
	 * 
	 * @param fov 
	 */
	void DecreaseFOV(float fov);
	/**
	 * @brief Get field of view
	 * 
	 * @return float 
	 */
	float GetFOV() const;
	/**
	 * @brief Set the Aspect Ratio of the Camera
	 * 
	 * @param width 
	 * @param height 
	 */
	void SetAspectRatio(size_t width, size_t height);
	/**
	 * @brief Set the Projection Type. There is perspective, orthographic, and orthographic viewport
	 * 
	 * @param projectionType 
	 */
	void SetProjectionType(CameraProjectionType projectionType);
	/**
	 * @brief Get the render type. Camera can render either 3D mesh, ui, or both
	 * 
	 * @return uint32_t 
	 */
	uint32_t GetCameraRenderObjectType() const;
	/**
	 * @brief Set the render type. Camera can render either 3D mesh, ui, or both
	 * 
	 * @param renderObjectType 
	 */
	void SetCameraRenderObjectType(uint32_t renderObjectType);
	/**
	 * @brief Set the Camera Position
	 * 
	 * @param x 
	 * @param y 
	 * @param z 
	 */
	void SetCameraPosition(float x, float y, float z);
	/**
	 * @brief Set the Camera Position
	 * 
	 * @param new_position 
	 */
	void SetCameraPosition(const Vector3& new_position);
	/**
	 * @brief Set the Camera Rotation
	 * 
	 * @param new_position 
	 */
	void SetCameraRotation(const Matrix& new_position);
	/**
	 * @brief Get the screen width & screen height that is associated with this camera
	 * 
	 * @return Vector2 
	 */
	Vector2 GetWidthHeight() const;
	/**
	 * @brief Get the Camera Position
	 * 
	 * @return Vector3 
	 */
	Vector3 GetCameraPosition() const;
	/**
	 * @brief Get Camera look at vector/forward vector
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetForward() const;
	/**
	 * @brief Get Camera Right vector
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetRight() const;

	void Reset();

	/**
	 * @brief Get the View Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	/**
	 * @brief Get the Inverse View Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetInvViewMatrix() const { return m_invViewMatrix; }
	/**
	 * @brief Get the Projection Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
	/**
	 * @brief Get the Inverse Projection Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetInvProjectionMatrix() const { return m_invProjectionMatrix; }
	/**
	 * @brief Get the View Projection Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	/**
	 * @brief Get the Inverse View Projection Matrix
	 * 
	 * @return const Matrix& 
	 */
	const Matrix& GetInvViewProjectionMatrix() const { return m_invViewProjectionMatrix; }
	
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	size_t GetId();
	std::string GetCameraName() const;
	/**
	 * @brief Get Camera Z Order. The higher the Z order means the render target of the Camera will be more on the top of the window screen
	 * 
	 * @return int32_t 
	 */
	int32_t GetZOrder() const;

	/**
	 * @brief Get the Viewport Render Information. This describe the size of the render target a camera should have
	 * 
	 * @return const Vector4& 
	 */
	const Vector4& GetViewportRenderInformation() const;

	/**
	 * @brief Get the Screen Width
	 * 
	 * @return size_t 
	 */
	size_t GetScreenWidth() const;
	/**
	 * @brief Get the Screen Height
	 * 
	 * @return size_t 
	 */
	size_t GetScreenHeight() const;
	/**
	 * @brief Set the Orthographic Width. Only for orthographic camera
	 * 
	 * @param width 
	 */
	void SetOrthographicWidth(size_t width);
	/**
	 * @brief Set the Orthographic Height. Only for orthographic camera
	 * 
	 * @param height 
	 */
	void SetOrthographicHeight(size_t height);
	/**
	 * @brief Get the Orthographic Width. Only for orthographic camera
	 * 
	 * @return size_t 
	 */
	size_t GetOrthographicWidth() const;
	/**
	 * @brief Get the Orthographic Height. Only for orthographic camera
	 * 
	 * @return size_t 
	 */
	size_t GetOrthographicHeight() const;
private:
	void InitFromComponent();
	void CalibrateFOV();
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

	size_t m_orthographicWidth;
	size_t m_orthographicHeight;

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

	size_t m_screenWidth;
	size_t m_screenHeight;

	CameraRenderObjectType m_cameraRenderObjectType;

	static size_t ID()
	{
		static size_t ID = 0;
		return ID++;
	}

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

