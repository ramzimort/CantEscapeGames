#include "Shading.h"

float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    float alpha_val = roughness * roughness;
    float phi = 2.f * PI * Xi.x;
    float cos_thetha = sqrt((1.0 - Xi.y) / (1.0 + (alpha_val * alpha_val - 1.0) * Xi.y));
    //the sin trigonometry identity 
    float sin_thetha = sqrt(1.0 - cos_thetha * cos_thetha);

    float3 H = float3(cos(phi) * sin_thetha, sin(phi) * sin_thetha, cos_thetha);

    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);

    float3 sample_vec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sample_vec);
}

//From learn opengl, to get the van der corpus sequence
float RadicalInverseVdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 HammersleySequence(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverseVdC(i));
}

float DistributionGGX(float3 normal_vector, float3 halfway_vector, float roughness)
{
    float alpha = roughness * roughness;
    float alpha_pow2 = alpha * alpha;
    float N_dot_H = max(dot(normal_vector, halfway_vector), 0.0);
    float N_dot_H_pow2 = N_dot_H * N_dot_H;

    float numerator = alpha_pow2;
    float denominator = (N_dot_H_pow2 * (alpha_pow2 - 1.f) + 1.f);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float GeometrySchlickGGX(float N_dot_V, float geometry_roughness)
{
    float denominator = N_dot_V * (1.f - geometry_roughness) + geometry_roughness;
    return N_dot_V / denominator;
}

float GeometrySmithIBL(float3 normal_vector, float3 light_vector, float3 viewer_vector, float roughness)
{
    float geometry_roughness = (roughness * roughness) / 2.f;

    float N_dot_V = max(dot(normal_vector, viewer_vector), 0.0);
    float N_dot_L = max(dot(normal_vector, light_vector), 0.0);

    float ggx2 = GeometrySchlickGGX(N_dot_V, geometry_roughness);
    float ggx1 = GeometrySchlickGGX(N_dot_L, geometry_roughness);

    return ggx1 * ggx2;
}


float GeometrySmith(float3 normal_vector, float3 light_vector, float3 viewer_vector, float roughness)
{
    float temp_r = roughness + 1.f;
    float geometry_roughness = (temp_r * temp_r) / 8.f;


    float N_dot_V = max(dot(normal_vector, viewer_vector), 0.0);
    float N_dot_L = max(dot(normal_vector, light_vector), 0.0);

    float ggx2 = GeometrySchlickGGX(N_dot_V, geometry_roughness);
    float ggx1 = GeometrySchlickGGX(N_dot_L, geometry_roughness);

    return ggx1 * ggx2;
}


float3 FresnelSchlickRoughness(float3 F0, float3 light_vector, float3 halfway_vector, float roughness)
{
    return F0 + (max(float3(1.f - roughness, 1.f - roughness, 1.f - roughness), F0) - F0) * pow((1.f - max(dot(light_vector, halfway_vector), 0.f)), 5.f);
}


float3 FresnelSchlick(float3 F0, float3 light_vector, float3 halfway_vector)
{
    return F0 + (float3(1.f, 1.f, 1.f) - F0) * pow((1.f - max(dot(light_vector, halfway_vector), 0.f)), 5.f);
}


float3 CookTorranceBRDF(float3 F0, float3 light_vector, float3 viewer_vector, float3 normal_vector, float metallic, float roughness, out float3 KD_term)
{
    float3 halfway_vector = normalize(light_vector + viewer_vector);

    float3 fresnel_vector = FresnelSchlick(F0, light_vector, halfway_vector);
    float D_factor = DistributionGGX(normal_vector, halfway_vector, roughness);
    float G_factor = GeometrySmith(normal_vector, light_vector, viewer_vector, roughness);

    float3 numerator = fresnel_vector * D_factor * G_factor;

    float denominator = 4.f * max(dot(normal_vector, viewer_vector), 0.0) * max(dot(normal_vector, light_vector), 0.0) + 0.001;

    float3 specular_term = numerator / denominator;

    KD_term = float3(1.f, 1.f, 1.f) - fresnel_vector;
    KD_term *= 1.f - metallic;

    return specular_term;
}


float3 PBRLightingCalculation(float3 light_vector, float3 light_color,
    float3 viewer_vector, float3 normal_vector, float3 F0, float3 albedo, float metallic, float roughness, bool enable_specular)
{
    float3 KD_term = float3(0.0, 0.0, 0.0);

    float3 specular_term = CookTorranceBRDF(F0, light_vector, viewer_vector, 
        normal_vector, metallic, roughness, KD_term);

    if(!enable_specular)
    {
        specular_term = float3(0.0, 0.0, 0.0);
    }

    
    float N_dot_L = max(dot(normal_vector, light_vector), 0.0);

    float3 final_lighting_calculation = (((KD_term * albedo) / PI) + specular_term) * light_color * N_dot_L;

    return final_lighting_calculation;
}