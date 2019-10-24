#include "Shading.h"
#include "PBR.hlsl"


#if SAMPLE_COUNT > 1
Texture2DMS<float4, SAMPLE_COUNT> WorldNormal_Texture : register(t1);
Texture2DMS<float4, SAMPLE_COUNT> Albedo_Texture : register(t2);
Texture2DMS<float4, SAMPLE_COUNT> Specular_Texture : register(t3);
Texture2DMS<float, SAMPLE_COUNT> Depth_Texture : register(t4);
Texture2DMS<float4, SAMPLE_COUNT> MaterialProperty_Texture : register(t5);
#else
Texture2D<float4> WorldNormal_Texture : register(t1);
Texture2D<float4> Albedo_Texture : register(t2);
Texture2D<float4> Specular_Texture : register(t3);
Texture2D<float> Depth_Texture : register(t4);
Texture2D<float4> MaterialProperty_Texture : register(t5);
#endif


struct PS_IN
{
    float4 Position : SV_Position;
    nointerpolation float3 LightPosition : TEXCOORD0;
    nointerpolation float3 LightColor : TEXCOORD1;
    nointerpolation float4 LightMiscData : TEXCOORD2;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};

cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};



void PhongCalculateTotalLightFactor(float3 world_normal, float3 world_position, float3 light_position, float3 light_color,
    out float3 args_total_diffuse_power, out float3 args_total_specular_power)
{
    float3 total_diffuse_power = float3(0.0, 0.0, 0.0);
    float3 total_specular_power = float3(0.0, 0.0, 0.0);

    float3 pixel_to_eye_diff = CameraUniformData_Buffer.CameraPosition.xyz - world_position;
    float3 pixel_to_eye_dir = normalize(pixel_to_eye_diff);

  
    float3 pixel_to_light_diff = light_position.xyz - world_position;
    float3 pixel_to_light_dir = normalize(pixel_to_light_diff);

    float diffuse_strength = saturate(dot(pixel_to_light_dir, world_normal));

    float3 final_diffuse_color = light_color.rgb * diffuse_strength;

    total_diffuse_power = final_diffuse_color;

    float3 halfway_dir = normalize(pixel_to_eye_dir + pixel_to_light_dir);

    float specular_cosine = saturate(dot(halfway_dir, world_normal));
    float specular_strength = pow(specular_cosine, 64.f);

    float3 final_specular_color = light_color.rgb * specular_strength;
    total_specular_power = final_specular_color;
    

    args_total_diffuse_power = total_diffuse_power;
    args_total_specular_power = total_specular_power;
}

float CalculatePolynomialAttenuation(float3 world_pos, float3 light_pos, float2 atten_constant)
{
    float3 diff = light_pos - world_pos;
    float r2 = dot(diff, diff);
    return saturate((r2 * atten_constant.x * (sqrt(r2) * atten_constant.y - 3.0f) + 1.0));
}

PS_OUT main(PS_IN ps_in, uint sample_index : SV_SampleIndex)
{
    PS_OUT ps_out;
    ps_out.Color = float4(0.0, 0.0, 0.0, 1.0);
    

#if SAMPLE_COUNT > 1
    float depth = Depth_Texture.Load(float2(ps_in.Position.xy), sample_index);
#else
    float depth = Depth_Texture.Load(float3(ps_in.Position.xy, 0.0));
#endif
    
    if(depth >= 1)
    {
        discard;
    }

    float2 screen_pos = ((ps_in.Position.xy / CameraUniformData_Buffer.CameraViewportSize.xy) * 2.0) - 1.0;
    screen_pos.y = -screen_pos.y;

    float4 position = mul(CameraUniformData_Buffer.InvViewProjectionMat, float4(screen_pos.xy, depth, 1.0));
    float3 world_position = position.xyz / position.w;

    float dist_to_light = distance(world_position, ps_in.LightPosition);

    const float light_radius = ps_in.LightMiscData.x;

    if (dist_to_light > light_radius)
    {
        discard;
    }
    

    float3 total_diffuse_power = float3(0.0, 0.0, 0.0);
    float3 total_specular_power = float3(0.0, 0.0, 0.0);
#if SAMPLE_COUNT > 1
    float3 world_normal = WorldNormal_Texture.Load(float2(ps_in.Position.xy), sample_index).xyz;
    float3 material_diffuse_color = Albedo_Texture.Load(float2(ps_in.Position.xy), sample_index).rgb;
    float3 material_specular_color = Specular_Texture.Load(float2(ps_in.Position.xy), sample_index).rgb;
    float2 material_property = MaterialProperty_Texture.Load(float2(ps_in.Position.xy), sample_index).rg;
#else
    float3 world_normal = WorldNormal_Texture.Load(float3(ps_in.Position.xy, 0.0)).xyz;
    float3 material_diffuse_color = Albedo_Texture.Load(float3(ps_in.Position.xy, 0.0)).rgb;
    float3 material_specular_color = Specular_Texture.Load(float3(ps_in.Position.xy, 0.0)).rgb;
    float2 material_property = MaterialProperty_Texture.Load(float3(ps_in.Position.xy, 0.0)).rg;
#endif
    float3 final_color = float3(0.0, 0.0, 0.0);

    const float light_intensity = ps_in.LightMiscData.y;
    const float2 light_atten_constant = ps_in.LightMiscData.zw;

    float light_atten = CalculatePolynomialAttenuation(world_position, ps_in.LightPosition, light_atten_constant);

    float3 light_illuminance = light_intensity * light_atten * ps_in.LightColor;

    float3 pixel_to_eye_diff = CameraUniformData_Buffer.CameraPosition.xyz - world_position;
    float3 viewer_vector = normalize(pixel_to_eye_diff);

    float3 pixel_to_light_diff = ps_in.LightPosition.xyz - world_position;
    float3 light_vector = normalize(pixel_to_light_diff);

    float roughness = material_property.r;
    float metallic = material_property.g;

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, material_specular_color.xyz, metallic);

   

    final_color.xyz = PBRLightingCalculation(light_vector, light_illuminance,
            viewer_vector, world_normal, F0, material_diffuse_color.xyz, metallic, roughness, true);

    ps_out.Color = float4(final_color.rgb, 1.0);

    return ps_out;
}