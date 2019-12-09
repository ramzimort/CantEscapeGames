#pragma once
#include "BaseComponent.h"
class GameObjectManager;

/**
 * @file TransformComponent.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Component that contains all data related to the position, orientation, and scale of any object in the world.
 */

/**
 * @brief Stores and updates all position, orientation and scale data
 * 
 */
class TransformComponent : public BaseComponent
{

//Friend classes and typedefs
public:
	friend class Factory;
	friend class TransformSystem;
public:
	TransformComponent(GameObject *owner);
	virtual ~TransformComponent();
	/**
	 * @brief Translate in world space
	 * 
	 * @param dx 
	 * @param dy 
	 * @param dz 
	 */
	void Translate(float dx, float dy, float dz);
	/**
	 * @brief Translate in world space
	 * 
	 * @param translate 
	 */
	void Translate(Vector3 const &translate);
	/**
	 * @brief Rotate with pitch, yaw, roll in degrees
	 * 
	 * @param pitch 
	 * @param yaw 
	 * @param roll 
	 */
	void Rotate(float pitch, float yaw, float roll);
	/**
	 * @brief Rotate with pitch, yaw, roll in degrees
	 * 
	 * @param rotation 
	 */
	void Rotate(Vector3 const& rotation);
	/**
	 * @brief Uniform scale
	 * 
	 * @param val 
	 */
	void Scale(float val);
	/**
	 * @brief Non-uniform scale
	 * 
	 * @param x 
	 * @param y 
	 * @param z 
	 */
	void Scale(float x, float y, float z);
	/**
	 * @brief Non-uniform scale
	 * 
	 * @param scale 
	 */
	void Scale(Vector3 const &scale);
	/**
	 * @brief Get the object's world space position
	 * 
	 * @return Vector3 const& 
	 */
	Vector3 const& GetPosition() const;
	/**
	 * @brief Get the object's rotation euler angles (in degrees)
	 * 
	 * @return Vector3 const& 
	 */
	Vector3 const& GetRotation() const;
	/**
	 * @brief Get the object's scale factors
	 * 
	 * @return Vector3 const& 
	 */
	Vector3 const& GetScale() const;
	/**
	 * @brief Get the obejct's model Matrix (computed per frame)
	 * 
	 * @return Matrix const& 
	 */
	Matrix const& GetModel() const;
	/**
	 * @brief Get the object's rotation matrix (computed per frame)
	 * 
	 * @return Matrix const& 
	 */
	Matrix const& GetRotationMatrix() const;
	/**
	 * @brief Get the object's scale matrix (computed every frame)
	 * 
	 * @return Matrix const& 
	 */
	Matrix const& GetScaleMatrix() const;
	/**
	 * @brief Gets the forward vector based on current orientation
	 * 
	 * @return Vector3 
	 */
	Vector3 GetForward() const;
	/**
	 * @brief Get the right vector based on current orientation
	 * 
	 * @return Vector3 
	 */
	Vector3 GetRight() const;
	Vector3  GetPositionNormalized() const;
	Vector3  GetScaleNormalized() const;
	/**
	 * @brief Set the position of the object in world space
	 * 
	 * @param x 
	 * @param y 
	 * @param z 
	 */
	void SetLocalPosition(float x, float y, float z);
	/**
	 * @brief Set the position of the object in world space
	 * 
	 * @param position 
	 */
	void SetLocalPosition(const Vector3& position);
	/**
	 * @brief Set the rotation directly with euler angles
	 * 
	 * @param eulerX 
	 * @param eulerY 
	 * @param eulerZ 
	 */
	void SetLocalRotation(float eulerX, float eulerY, float eulerZ); 
	
	//Only for Shooter's gameplay logic for now.
	//Not for public interface usage
	void SetLocalRotationMatrix(const Matrix& rotationMatrix);
private:
	//Setup methods
	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;
	
	//dirty flag
	void SetModel(Matrix const& model);
	bool NeedToRecalculate();
	void ResetDirtyFlag();

public:
	//To compare when using templates
	static ComponentId const static_type;

private:
	//Flag for knowing if the object moved
	bool m_needToRecalculateModel;
	//Only for Shooter's gameplay logic for now.
	//Not for public interface usage
	bool m_rotMatrixAutomaticCalc;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	Matrix m_model;
	Matrix m_rotMatrix;
	Matrix m_scaleMatrix;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};