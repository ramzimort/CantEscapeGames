#include "Shading.h"
#include "PBR.hlsl"

TextureCube<float4> SourceTexture : register(t0);
RWTexture2DArray<float4> DestTexture : register(u0);

SamplerState SkyboxSampler : register(s0);


cbuffer IBLFilterEnvMapUniformData_CB : register(b0)
{
    IBLFilterEnvMapUniformData IBLFilterEnvMapUniformData_Buffer;
};




[numthreads(IBL_ENV_MAP_FILTER_WORKGROUP_THREAD_ID_X, IBL_ENV_MAP_FILTER_WORKGROUP_THREAD_ID_Y, IBL_ENV_MAP_FILTER_WORKGROUP_THREAD_ID_Z)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    float4 mip_size = IBLFilterEnvMapUniformData_Buffer.SkyboxSize;
    float2 texcoords = float2(float(DispatchThreadID.x + 0.5) / mip_size.x,(float(DispatchThreadID.y + 0.5) / mip_size.y));

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
        sphere_dir = normalize(float3(texcoords.x - 0.5, 0.5, (texcoords.y - 0.5)));
    }
    else if (DispatchThreadID.z <= 3)
    {
        sphere_dir = normalize(float3(texcoords.x - 0.5, -0.5, -(texcoords.y - 0.5)));
    }
    else if (DispatchThreadID.z <= 4)
    {
        sphere_dir = normalize(float3(texcoords.x - 0.5, -(texcoords.y - 0.5), 0.5));
    }
    else if (DispatchThreadID.z <= 5)
    {
        sphere_dir = normalize(float3(-(texcoords.x - 0.5), -(texcoords.y - 0.5), -0.5));
    }
    float3 R_term = sphere_dir;
    float3 V_term = sphere_dir;
    float roughness = IBLFilterEnvMapUniformData_Buffer.MiscData.x;

    float total_weight = 0.f;
    float3 prefiltered_color = float3(0.0f, 0.0f, 0.0f);

    
    uint sample_count = 1024u;
    for (uint i = 0u; i < sample_count; ++i)
    {
        float2 Xi = HammersleySequence(i, sample_count);
        float3 H_term = ImportanceSampleGGX(Xi, sphere_dir, roughness);
        //reflection on halfway vector term based on the sphere direction
        float3 L_term = normalize(2.0 * dot(V_term, H_term) * H_term - V_term);
        float N_dot_L = max(dot(sphere_dir, L_term), 0.0);

        if(N_dot_L > 0.0)
        {
            float D = DistributionGGX(sphere_dir, H_term, roughness);
            float N_dot_H = max(dot(sphere_dir, H_term), 0.0);
            float H_dot_V = max(dot(H_term, V_term), 0.0);
            float pdf = D * N_dot_H / (4.0 * H_dot_V) + 0.0001;

            float resolution_width = IBLFilterEnvMapUniformData_Buffer.SourceSkyboxSize.x;
            float resolution_height = IBLFilterEnvMapUniformData_Buffer.SourceSkyboxSize.y;
            float saTexel = 4.0 * PI / (6.0 * resolution_width * resolution_height);
            float saSample = 1.0 / (float(sample_count) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefiltered_color += SourceTexture.SampleLevel(SkyboxSampler, L_term, mipLevel).rgb * N_dot_L;
            total_weight += N_dot_L;
        }
    }
    
    prefiltered_color /= total_weight;

    //float3 skybox_color = SourceTexture.SampleLevel(SkyboxSampler, sphere_dir, IBLFilterEnvMapUniformData_Buffer.MipLevel).rgb;

    DestTexture[DispatchThreadID] = float4(prefiltered_color, 1.0);
}

