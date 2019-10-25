#pragma once

class HaloEffect
{
public:
	friend class AppRenderer;
	friend class DeferredRendering;
	friend class HaloEffectSystem;
public:
	HaloEffect();
	~HaloEffect();

	void SetRadius(float radius);
	void SetIntensity(float intensity);
	void SetColor(const Vector3& color);

	const Vector3& GetColor() const { return m_color; };
	float GetIntensity() const { return m_intensity; };
	float GetRadius() const { return m_radius; };

private:
	//relative halo position to game object
	Vector3 m_position;
	Vector3 m_color;
	float m_intensity;
	float m_radius;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

