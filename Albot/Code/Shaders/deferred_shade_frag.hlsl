#include "Shading.h"

//Texture2D<float4> WorldPosition_Texture : register(t0);
Texture2D<float4> WorldNormal_Texture : register(t0);
Texture2D<float4> Albedo_Texture : register(t1);
Texture2D<float4> Specular_Texture : register(t2);
Texture2D<float> Depth_Texture : register(t3);
Texture2D<float> ShadowMap_Texture : register(t4);


SamplerState Clamp_Billinear_Sampler : register(s0);

SamplerComparisonState Shadow_Sampler : register(s1);

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

float PCF(float3 shadow_coord, float2 kernelSize)
{
    float depth_bias = 0.0f;
    float2 tap_offset[9] =
    {
        float2(0.00, 0.00),
        float2(1.20, 0.00),
        float2(-1.20, 0.00),
        float2(0.00, 1.20),
        float2(0.00, -1.20),
        float2(0.84, 0.84),
        float2(-0.84, 0.84),
        float2(-0.84, -0.84),
        float2(0.84, -0.84),
    };

    float tap_weight[9] =
    {
        0.120892,
        0.110858,
        0.110858,
        0.110858,
        0.110858,
        0.111050,
        0.111050,
        0.111050,
        0.111050,
    };

    float shadow_factor = 0;
    for (int i = 0; i < 9; ++i)
    {
        float2 sample_coord = shadow_coord.xy + kernelSize * tap_offset[i];
        shadow_factor += tap_weight[i] * ShadowMap_Texture.SampleCmpLevelZero(Shadow_Sampler, sample_coord, shadow_coord.z - depth_bias);
    }
    return shadow_factor;
}


float CalculateOrthographicShadowFactor(float3 world_position)
{
    uint total_light_count = (uint) (DirectionLightUniform.DirectionalLightUniformMiscData.w);

    if(total_light_count <= 0)
    {
        return 0.0f;
    }

    float4 shadow_world_pos = mul(LightCameraUniformData_Buffer.ViewProjectionMat, float4(world_position, 1.0)).xyzw;
    float3 projected_world_pos = shadow_world_pos.xyz / shadow_world_pos.w; 

    if (projected_world_pos.x <= -1.0f || projected_world_pos.x >= 1.0f ||
		projected_world_pos.y <= -1.0f || projected_world_pos.y >= 1.0f ||
		projected_world_pos.z <= 0.0f || projected_world_pos.z >= 1.0f
		)
    {
        return 0.0f;
    }

    float2 projected_UV = float2(0.5f, 0.5f) + (float2(0.5f, -0.5f) * projected_world_pos.xy);

    float3 shadow_map_coord = float3(projected_UV.xy, projected_world_pos.z);
    float2 kernel_size = float2(1.f / LightCameraUniformData_Buffer.CameraViewportSize.x, 1.f / LightCameraUniformData_Buffer.CameraViewportSize.y);


    float shadow_intensity = DirectionLightUniform.DirectionalLightMiscData[0].z;

    return PCF(shadow_map_coord, kernel_size) * shadow_intensity;
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




float LinearizeDepth(float depth)
{
    float near_plane = 0.1f;
    float far_plane = 1000.f;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
    float depth = Depth_Texture.Load(float3(ps_in.Position.xy, 0.f)).r;
    
    if(depth >= 1.f)
    {
        discard;
    }

    float ambient_strength = 0.15f;


    float2 clip_pixel = float2((ps_in.UV.x * 2.f) - 1, 1.f - (ps_in.UV.y * 2.f));

    float4 position = mul(CameraUniformData_Buffer.InvViewProjectionMat, float4(clip_pixel.xy, depth, 1.0));
    float3 world_position = position.xyz / position.w;

    float shadow_factor = CalculateOrthographicShadowFactor(world_position);

    float3 albedo = Albedo_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;
    float3 world_normal = WorldNormal_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;
    float3 specular = Specular_Texture.Load(float3(ps_in.Position.xy, 0.0f)).rgb;

    float3 total_diffuse_power = float3(0.0, 0.0, 0.0);
   


    PhongCalculateTotalLightFactor(world_normal, world_position, total_diffuse_power);

    total_diffuse_power *=  (1.0 - shadow_factor);

    //float3 final_color = total_diffuse_power * albedo;
    float3 final_color = ((PI * ambient_strength * albedo)
        + total_diffuse_power) * albedo * (1.f / PI);

    float linear_depth = LinearizeDepth(depth) / 1000.f;
   

    ps_out.Color = float4(final_color.rgb, 1.0);
    //ps_out.Color = float4(linear_depth, linear_depth, linear_depth, 1.0);
    //ps_out.Color = float4(world_normal.rgb, 1.0);
    return ps_out;
}


 //ps_out.Color = float4(depth, depth, depth, 1.0);
  
    //float3 world_position = WorldPosition_Texture.Load(float3(ps_in.Position.xy, 0.f));
    //float3 world_position = WorldNormal_Texture.Sample(Clamp_Billinear_Sampler, ps_in.UV).xyz;
    //world_position = float3(ps_in.UV.xy, 0.0);
    //ps_out.Color = float4(world_position, 1.0);