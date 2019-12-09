#pragma once
/**
 * @file HaloEffect.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief Halo Effect emit glowing 3D volumetric light around a game object
 */

/**
 * @brief Defines the interface for HaloEffect
 * 
 */
class HaloEffect
{
public:
	friend class AppRenderer;
	friend class DeferredRendering;
	friend class HaloEffectSystem;
public:
	HaloEffect();
	~HaloEffect();

	/**
	* @brief Set the Radius object
	* 
	* @param radius 
	*/
	void SetRadius(float radius);
	/**
	 * @brief Set the Intensity object
	 * 
	 * @param intensity 
	 */
	void SetIntensity(float intensity);
	/**
	 * @brief Set the Color object
	 * 
	 * @param color 
	 */
	void SetColor(const Vector3& color);

	/**
	 * @brief Get the Position object
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetPosition() const { return m_position; }
	/**
	 * @brief Get the Color object
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetColor() const { return m_color; };
	/**
	 * @brief Get the Intensity object
	 * 
	 * @return float 
	 */
	float GetIntensity() const { return m_intensity; };
	/**
	 * @brief Get the Radius object
	 * 
	 * @return float 
	 */
	float GetRadius() const { return m_radius; };
	/**
	 * @brief Set the Position of the Halo Effect relative to its GameObject's TransformComponent's position
	 * 
	 * @param pos 
	 */
	void SetPosition(const Vector3& pos) { m_position = pos; }
private:
	//relative halo position to game object
	Vector3 m_position;
	Vector3 m_color;
	float m_intensity;
	float m_radius;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

