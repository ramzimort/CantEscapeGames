#include "Shading.h"

RWTexture2D<float> OutTexture : register(u0);
Texture2D<float> InputTexture : register(t0);


cbuffer BlurUniformData_CB : register(b0)
{
    BlurUniformData BlurUniformData_Buffer;
};

groupshared float SharedPixelData[BLUR_WORKGROUP_THREAD_ID_X + 101];

[numthreads(BLUR_WORKGROUP_THREAD_ID_X, BLUR_WORKGROUP_THREAD_ID_Y, BLUR_WORKGROUP_THREAD_ID_Z)]
void main(uint3 GroupID : SV_GroupID,
	uint3 DispatchThreadID : SV_DispatchThreadID,
    uint3 GroupThreadID : SV_GroupThreadID)
{
    uint2 direction = uint2((uint) BlurUniformData_Buffer.DirectionX, (uint) BlurUniformData_Buffer.DirectionY);

    uint2 cur_input_pixel_loc = uint2(DispatchThreadID.x * direction + DispatchThreadID.y * (uint2(1, 1) - direction));
    uint cur_output_texel_index = GroupThreadID.x;


    int2 first_input_texel_index = cur_input_pixel_loc - 
        BlurUniformData_Buffer.BlurWidth * direction;


    SharedPixelData[cur_output_texel_index] = InputTexture.Load(uint3(first_input_texel_index.x, first_input_texel_index.y, 0)).r;

    if (cur_output_texel_index < BlurUniformData_Buffer.BlurWidth2)
    {
        SharedPixelData[cur_output_texel_index + BLUR_WORKGROUP_THREAD_ID_X] = InputTexture.Load(uint3(first_input_texel_index + BLUR_WORKGROUP_THREAD_ID_X * direction, 0)).r;
    }

    GroupMemoryBarrierWithGroupSync();

    float result = 0.0;
    for (uint i = 0; i < BlurUniformData_Buffer.BlurWidth2; ++i)
    {
        result += BlurUniformData_Buffer.BlurWeight[i].x * SharedPixelData[i + cur_output_texel_index];
    }
    OutTexture[cur_input_pixel_loc.xy] = result;
}