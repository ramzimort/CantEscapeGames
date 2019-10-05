#include "Shading.h"

struct PS_IN
{
    float4 Position : SV_Position;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};



cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};

cbuffer MSAAResolveUniform_CB :  register(b1)
{
    MSAAResolveUniformData MSAAResolveUniformData_Buffer;
};




Texture2DMS<float4, SAMPLE_COUNT> MSAA_Texture : register(t0);

float FilterCubic(in float x, in float B, in float C)
{
    x *= 2.0f;

    float y = 0.0f;
    float x2 = x * x;
    float x3 = x * x * x;
    if (x < 1)
    {
        y = (12 - 9 * B - 6 * C) * x3 + (-18 + 12 * B + 6 * C) * x2 + (6 - 2 * B);
    }
    else if (x <= 2)
    {
        y = (-B - 6 * C) * x3 + (6 * B + 30 * C) * x2 + (-12 * B - 48 * C) * x + (8 * B + 24 * C);
    }

    return y / 6.0f;
}


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;

    float4 final_color = 0;

    uint2 UV = uint2(ps_in.Position.xy);

    float4 total_color = 0.0f;
    float total_weight = 0.0f;
    float2 pixel_pos = ps_in.Position.xy;


    int SampleRadius = (int) MSAAResolveUniformData_Buffer.SampleRadius;

    for (int y = -SampleRadius; y <= SampleRadius; ++y)
    {
        for (int x = -SampleRadius; x <= SampleRadius; ++x)
        {
            float2 offsets = float2(x, y);
            float2 cur_sample_pos = pixel_pos + offsets;

            cur_sample_pos = clamp(cur_sample_pos, float2(0.f, 0.f), float2(CameraUniformData_Buffer.CameraViewportSize.x - 1,
                CameraUniformData_Buffer.CameraViewportSize.y - 1));
            for (uint sample_index = 0; sample_index < SAMPLE_COUNT; ++sample_index)
            {
                
                offsets += MSAAResolveUniformData_Buffer.SampleOffsets[sample_index];
                float sampler_dist = length(offsets) / (MSAAResolveUniformData_Buffer.FilterSize / 2.0);

                if(sampler_dist <= 1.0f)
                {
                    
                    float4 cur_sampled_color = MSAA_Texture.Load(cur_sample_pos, sample_index).rgba;

                    //float weight = FilterCubic(sampler_dist, 1.f, 0.f);
                    float weight = FilterCubic(sampler_dist, 1 / 3.0f, 1 / 3.0f);
                    //float weight = FilterCubic(sampler_dist, 0, 0.5f);
                    total_color += cur_sampled_color.rgba * weight;
                    total_weight += weight;
                }

            }

        }

    }
    
    total_color = total_color / max(total_weight, 0.00001f);
    total_color = max(total_color, 0.0f);

    final_color = float4(total_color.rgba);
    ps_out.Color = final_color;

    return ps_out;
}
