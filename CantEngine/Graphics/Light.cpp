#include "Light.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Light>("Light")
		.property("LightType", &Light::m_light_type)
		.property("Position", &Light::m_position)
		.property("Color", &Light::m_color)
		.property("Intensity", &Light::m_intensity)
		.property("CastShadow", &Light::m_cast_shadow)
		.property("Enabled", &Light::m_enabled)
		.property("Radius", &Light::m_radius)
		.property("ShadowOrthographicSize", &Light::m_shadowOrthographicSize)
		.property("ShadowCasterDistanceFromFocusPoint", &Light::m_shadowCasterDistanceFromFocusPoint)
		.property("ShadowIntensity", &Light::m_shadow_intensity);

	rttr::registration::enumeration<ELightType>("LightType")(
		rttr::value("Directional", ELightType::DIRECTIONAL_LIGHT),
		rttr::value("Point", ELightType::POINT_LIGHT));
}


Light::Light()
	:m_light_type(ELightType::POINT_LIGHT),
	m_color(1.f, 1.f, 1.f),
	m_intensity(1.f),
	m_cast_shadow(true),
	m_enabled(true),
	m_radius(1.f),
	m_shadow_intensity(1.f),
	m_position(0.f),
	m_shadowOrthographicSize(75.f, 75.f),
	m_shadowCasterDistanceFromFocusPoint(65.f)
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

ELightType Light::GetLightType() const
{
	return m_light_type;
}

float Light::GetRadius() const
{
	return m_radius;
}

Vector2 Light::GetAttenuationConstant() const
{
	Vector2 atten_cost (1.0f / (m_radius * m_radius), 2.0f / m_radius);
	return atten_cost;
}