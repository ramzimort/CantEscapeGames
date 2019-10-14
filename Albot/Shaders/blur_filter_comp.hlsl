#include "Shading.h"

#if BLUR_CHANNEL_TYPE == 0
RWTexture2D<float> OutTexture : register(u0);
Texture2D<float> InputTexture : register(t0);
#elif BLUR_CHANNEL_TYPE == 3
RWTexture2D<float4> OutTexture : register(u0);
Texture2D<float4> InputTexture : register(t0);
#endif


cbuffer BlurUniformData_CB : register(b0)
{
    BlurUniformData BlurUniformData_Buffer;
};

#if BLUR_CHANNEL_TYPE == 0
groupshared float SharedPixelData[BLUR_WORKGROUP_THREAD_ID_X + 101];
#elif BLUR_CHANNEL_TYPE == 3
groupshared float4 SharedPixelData[BLUR_WORKGROUP_THREAD_ID_X + 101];
#endif

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

#if BLUR_CHANNEL_TYPE == 0
    SharedPixelData[cur_output_texel_index] = InputTexture.Load(uint3(first_input_texel_index.x, first_input_texel_index.y, 0)).r;
#elif BLUR_CHANNEL_TYPE == 3
    SharedPixelData[cur_output_texel_index] = InputTexture.Load(uint3(first_input_texel_index.x, first_input_texel_index.y, 0)).rgba;
#endif

    if (cur_output_texel_index < BlurUniformData_Buffer.BlurWidth2)
    {
#if BLUR_CHANNEL_TYPE == 0
        SharedPixelData[cur_output_texel_index + BLUR_WORKGROUP_THREAD_ID_X] = InputTexture.Load(uint3(first_input_texel_index + BLUR_WORKGROUP_THREAD_ID_X * direction, 0)).r;
#elif BLUR_CHANNEL_TYPE == 3
        SharedPixelData[cur_output_texel_index + BLUR_WORKGROUP_THREAD_ID_X] = InputTexture.Load(uint3(first_input_texel_index + BLUR_WORKGROUP_THREAD_ID_X * direction, 0)).rgba;
#endif
    }

    GroupMemoryBarrierWithGroupSync();
#if BLUR_CHANNEL_TYPE == 0
    float result = 0.0;
#elif BLUR_CHANNEL_TYPE == 3
    float4 result = float4(0.f, 0.f, 0.f, 0.f);
#endif
    for (uint i = 0; i < BlurUniformData_Buffer.BlurWidth2; ++i)
    {
#if BLUR_CHANNEL_TYPE == 0
        result += BlurUniformData_Buffer.BlurWeight[i].x * SharedPixelData[i + cur_output_texel_index];
#elif BLUR_CHANNEL_TYPE == 3
        result += BlurUniformData_Buffer.BlurWeight[i].x * SharedPixelData[i + cur_output_texel_index].xyzw;
#endif
    }
    OutTexture[cur_input_pixel_loc.xy] = result;
}