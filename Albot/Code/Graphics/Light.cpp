#include "Light.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Light>("Light")
		.property("Mass", &Light::m_light_type)
		.property("Dynamic", &Light::m_color)
		.property("Bounciness", &Light::m_intensity)
		.property("Collision_Mask", &Light::m_cast_shadow)
		.property("Friction_Coeff", &Light::m_enabled)
		.property("Shape", &Light::m_radius)
		.property("Destroyable", &Light::m_shadow_intensity);

	rttr::registration::enumeration<ELightType>("LightType")(
		rttr::value("Wall", ELightType::DIRECTIONAL_LIGHT),
		rttr::value("Dynamic", ELightType::ELIGHT_TYPE_TOTAL_COUNT),
		rttr::value("Static", ELightType::POINT_LIGHT),
		rttr::value("Player", ELightType::SPOT_LIGHT));
}


Light::Light()
	:m_light_type(ELightType::POINT_LIGHT),
	m_color(1.f, 1.f, 1.f),
	m_intensity(1.f),
	m_cast_shadow(true),
	m_enabled(true),
	m_radius(1.f),
	m_shadow_intensity(1.f)
{
}


Light::~Light()
{
}

const Vector3& Light::GetColor() const
{
	return m_color;
}

float Light::GetIntensity() const
{
	return m_intensity;
}

ELightType Light::get_light_type() const
{
	return m_light_type;
}

float Light::get_radius() const
{
	return m_radius;
}

Vector2 Light::get_attenuation_constant() const
{
	Vector2 atten_cost (1.0f / (m_radius * m_radius), 2.0f / m_radius);
	return atten_cost;
}