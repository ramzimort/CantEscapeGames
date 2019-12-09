#include "Shading.h"

Texture2D<float4> SourceTexture : register(t0);
RWTexture2DArray<float4> DestTexture : register(u0);

SamplerState SkyboxSampler : register(s0);


cbuffer SourceTextureToSkyboxUniformData_CB : register(b0)
{
    SourceTextureToSkyboxUniformData SourceTextureToSkyboxUniformData_Buffer;
};


float2 SampleSphericalMap(float3 local_pos)
{
    float2 UV = float2(atan2(local_pos.z, local_pos.x), asin(local_pos.y));
    UV *= float2(0.1591f, 0.3183f);
    UV += float2(0.5f, 0.5f);
    return UV;

}

[numthreads(TO_SKYBOX_WORKGROUP_THREAD_ID_X, TO_SKYBOX_WORKGROUP_THREAD_ID_Y, TO_SKYBOX_WORKGROUP_THREAD_ID_Z)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    float4 mip_size = SourceTextureToSkyboxUniformData_Buffer.SkyboxSize;
    float2 texcoords = float2( float(DispatchThreadID.x + 0.5) / mip_size.x, 1.0 - ( float(DispatchThreadID.y + 0.5) / mip_size.y));

    float3 sphere_dir = float3(0.0, 0.0, 0.0);
    if (DispatchThreadID.z <= 0)
    {
        sphere_dir = normalize(float3(0.5, -(texcoords.y - 0.5), -(texcoords.x - 0.5)));
    }
    else if (DispatchThreadID.z <= 1)
    {
        sphere_dir = normalize(float3(-0.5, -(texcoords.y - 0.5), texcoords.x - 0.5));
    }
    else if (DispatchThreadID.z <= 2)
    {
        sphere_dir = normalize(float3(texcoords.x - 0.5, -0.5, -(texcoords.y - 0.5)));
    }
    else if (DispatchThreadID.z <= 3)
    {
        sphere_dir = normalize(float3(texcoords.x - 0.5, 0.5, texcoords.y - 0.5));
    }
    else if (DispatchThreadID.z <= 4)
    {
        sphere_dir = normalize(float3(texcoords.x - 0.5, -(texcoords.y - 0.5), 0.5));
    }
    else if (DispatchThreadID.z <= 5)
    {
        sphere_dir = normalize(float3(-(texcoords.x - 0.5), -(texcoords.y - 0.5), -0.5));
    }

    float2 equirectangular_uv =  SampleSphericalMap(sphere_dir);
    DestTexture[DispatchThreadID] = SourceTexture.SampleLevel(SkyboxSampler, equirectangular_uv, SourceTextureToSkyboxUniformData_Buffer.MipLevel).rgba;
}