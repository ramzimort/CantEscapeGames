#include "Shading.h"
#include "moment_shadow.hlsl"
#include "PBR.hlsl"


#if SAMPLE_COUNT > 1
Texture2DMS<float4, SAMPLE_COUNT> WorldNormal_Texture : register(t0);
Texture2DMS<float4, SAMPLE_COUNT> Albedo_Texture : register(t1);
Texture2DMS<float4, SAMPLE_COUNT> Specular_Texture : register(t2);
Texture2DMS<float, SAMPLE_COUNT> Depth_Texture : register(t3);
Texture2DMS<float4, SAMPLE_COUNT> MaterialProperty_Texture : register(t5);
#else
Texture2D<float4> WorldNormal_Texture : register(t0);
Texture2D<float4> Albedo_Texture : register(t1);
Texture2D<float4> Specular_Texture : register(t2);
Texture2D<float> Depth_Texture : register(t3);
Texture2D<float4> MaterialProperty_Texture : register(t5);
#endif

Texture2D<float4> ShadowMap_Texture : register(t4);

SamplerState Clamp_Billinear_Sampler : register(s0);

#if SKYBOX_IRRADIANCE == 1
TextureCube<float4> Irradiance_Texture : register(t6);
TextureCube<float4> IBLFilteredEnvMap_Texture : register(t7);
Texture2D<float2> BRDFLookup_Texture : register(t8);

SamplerState Trillinear_Sampler : register(s1);
#endif

cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


cbuffer DirectionalLightUniformBuffer : register(b1)
{
    DirectionalLightUniformData DirectionLightUniform;
};


cbuffer LightCameraUniformData_CB : register(b2)
{
    CameraUniformData LightCameraUniformData_Buffer;
};



struct PS_IN
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};



float CalculateShadowFactor(float3 world_position)
{
    uint total_light_count = (uint) (DirectionLightUniform.DirectionalLightUniformMiscData.w);
    if (total_light_count <= 0)
    {
        return 0.0f;
    }

    float4 shadow_world_pos = mul(LightCameraUniformData_Buffer.ViewProjectionMat, float4(world_position, 1.0)).xyzw;
    float3 projected_world_pos = shadow_world_pos.xyz / shadow_world_pos.w;

    if (projected_world_pos.x <= -0.99f || projected_world_pos.x >= 0.99f ||
		projected_world_pos.y <= -0.99f || projected_world_pos.y >= 0.99f ||
		projected_world_pos.z <= 0.0f || projected_world_pos.z >= 1.0f
		)
    {
        return 0.0f;
    }

    float2 projected_UV = float2(0.5f, 0.5f) + (float2(0.5f, -0.5f) * projected_world_pos.xy);
    
    float4 moments_value = ShadowMap_Texture.Sample(Clamp_Billinear_Sampler, float2(projected_UV.xy)).rgba;

    return CalculateMSM(moments_value, projected_world_pos.z);
}





void PhongCalculateTotalLightFactor(float3 world_normal, float3 world_position, out float3 args_total_diffuse_power)
{
    uint total_light_count = (uint) (DirectionLightUniform.DirectionalLightUniformMiscData.w);

    float3 total_diffuse_power = float3(0.0, 0.0, 0.0);
   
    float3 pixel_to_eye_diff = CameraUniformData_Buffer.CameraPosition.xyz - world_position;
    float3 pixel_to_eye_dir = normalize(pixel_to_eye_diff);

    for (uint i = 0; i < total_light_count; ++i)
    {
        float3 light_color = DirectionLightUniform.DirectionalLightColor[i].rgb * DirectionLightUniform.DirectionalLightMiscData[i].y;
        float3 pixel_to_light_dir = -DirectionLightUniform.LightDirection[i].xyz;

        float diffuse_strength = saturate(dot(pixel_to_light_dir, world_normal));

        float3 final_diffuse_color = light_color * diffuse_strength;

        total_diffuse_power += final_diffuse_color;
    }

    args_total_diffuse_power = total_diffuse_power;
}


void CalculateDirectionalLightFactor(float3 world_normal, float3 world_position, float3 viewer_vector, float3 albedo, float3 irradiance, float3 F0,
    float metallic, float roughness, out float3 light_power)
{
    uint total_light_count = (uint) (DirectionLightUniform.DirectionalLightUniformMiscData.w);
    float3 final_color = float3(0.0, 0.0, 0.0);

    for (uint i = 0; i < total_light_count; ++i)
    {
        float3 light_color = irradiance * DirectionLightUniform.DirectionalLightColor[i].rgb * DirectionLightUniform.DirectionalLightMiscData[i].y;
        float3 light_vector = -DirectionLightUniform.LightDirection[i].xyz;

        final_color += PBRLightingCalculation(light_vector, light_color,
            viewer_vector, world_normal, F0, albedo.xyz, metallic, roughness, true);
    }
    light_power = final_color;
}

PS_OUT main(PS_IN ps_in, uint sample_index : SV_SampleIndex)
{
    PS_OUT ps_out;
#if SAMPLE_COUNT > 1
    float depth = Depth_Texture.Load(float2(ps_in.Position.xy), sample_index).r;
#else
    float depth = Depth_Texture.Load(float3(ps_in.Position.xy, 0.f)).r;
#endif
    
    if(depth >= 1.f)
    {
        discard;
    }

    float ambient_strength = 0.1f;
    float2 clip_pixel = float2((ps_in.UV.x * 2.f) - 1, 1.f - (ps_in.UV.y * 2.f));

    float4 position = mul(CameraUniformData_Buffer.InvViewProjectionMat, float4(clip_pixel.xy, depth, 1.0));
    float3 world_position = position.xyz / position.w;

  
    float shadow_factor = 0.f;
    shadow_factor = CalculateShadowFactor(world_position);
  
#if SAMPLE_COUNT > 1
    float3 albedo = Albedo_Texture.Load(float2(ps_in.Position.xy), sample_index).rgb;
    float3 world_normal = WorldNormal_Texture.Load(float2(ps_in.Position.xy), sample_index).rgb;
    float3 specular = Specular_Texture.Load(float2(ps_in.Position.xy), sample_index).rgb;
    float2 material_property = MaterialProperty_Texture.Load(float2(ps_in.Position.xy), sample_index).rg;
#else
    float3 albedo = Albedo_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;
    float3 world_normal = WorldNormal_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;
    float3 specular = Specular_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;
    float2 material_property = MaterialProperty_Texture.Load(float3(ps_in.Position.xy, 0.0)).rg;
#endif
    float3 final_color = float3(0.0, 0.0, 0.0);

    float3 irradiance = float3(1.0, 1.0, 1.0);
    float3 total_light_strength = float3(0.0, 0.0, 0.0);
    float roughness = material_property.r;
    float metallic = material_property.g;

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo.xyz, metallic);

    float3 pixel_to_eye_diff = CameraUniformData_Buffer.CameraPosition.xyz - world_position;
    float3 viewer_vector = normalize(pixel_to_eye_diff);
#if SKYBOX_IRRADIANCE == 1
    irradiance = Irradiance_Texture.Sample(Clamp_Billinear_Sampler, world_normal).rgb;

    CalculateDirectionalLightFactor(world_normal, world_position, viewer_vector, albedo, irradiance, F0, metallic, roughness, total_light_strength);

    total_light_strength *= (1.f - shadow_factor);

    float3 F_roughness_term = FresnelSchlickRoughness(F0, world_normal, viewer_vector, roughness);
    F_roughness_term = max(F_roughness_term, float3(0.0, 0.0, 0.0));
    float3 KS_term = F_roughness_term;
    float3 KD_term = float3(1.f, 1.f, 1.f) - KS_term;
    KD_term *= float3(1.f, 1.f, 1.f) - float3(metallic, metallic, metallic);

    float3 reflected_L = reflect(-viewer_vector, world_normal);

    float N_dot_V = max(dot(world_normal, viewer_vector), 0);

    float3 prefiltered_color = IBLFilteredEnvMap_Texture.SampleLevel(Trillinear_Sampler, float3(reflected_L.xyz), roughness * 4.0).rgb;
    float2 brdf_factor = BRDFLookup_Texture.SampleLevel(Clamp_Billinear_Sampler, float2(N_dot_V, roughness), 0).rg;

    float3 specular_part = prefiltered_color * (F_roughness_term * brdf_factor.x + brdf_factor.y);

    float3 ambient_color = KD_term * albedo * irradiance * ambient_strength;

    specular_part *= (1.f - shadow_factor);

    final_color = ambient_color + total_light_strength + specular_part;

    
#else
    float3 ambient_color = albedo.rgb * ambient_strength;
    CalculateDirectionalLightFactor(world_normal, world_position, viewer_vector, albedo, irradiance, F0, metallic, roughness, total_light_strength);

    total_light_strength *= (1.f - shadow_factor);

    final_color = ambient_color + total_light_strength;
#endif

    final_color = final_color / (final_color + float3(1.f, 1.f, 1.f));
    final_color = pow(final_color, float3(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f));
   
    ps_out.Color = float4(final_color.rgb, 1.0);
    return ps_out;
}


 //ps_out.Color = float4(depth, depth, depth, 1.0);
  
    //float3 world_position = WorldPosition_Texture.Load(float3(ps_in.Position.xy, 0.f));
    //float3 world_position = WorldNormal_Texture.Sample(Clamp_Billinear_Sampler, ps_in.UV).xyz;
    //world_position = float3(ps_in.UV.xy, 0.0);
    //ps_out.Color = float4(world_position, 1.0);