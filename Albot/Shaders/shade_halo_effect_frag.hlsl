#include "Shading.h"

Texture2DMS<float4> StructureBuffer_Texture : register(t1);

struct PS_IN
{
    float4 Position : SV_Position;
    float3 ObjectSpacePos : OBJECT_SPACE_POS;
    float3 CameraPosObjectSpace : CAMERA_OBJ_SPACE;
    float3 CameraZAxisDirObjectSpace : CAMERA_DIR_SPACE;
    nointerpolation float3 HaloColor : HALO_COLOR;
    nointerpolation float4 HaloMiscData : HALO_MISC_DATA;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};

cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


float CalculateHaloFactor(PS_IN ps_in)
{
    float r_pow2 = ps_in.HaloMiscData.x;
    float3 view_direction = ps_in.CameraPosObjectSpace.xyz - ps_in.ObjectSpacePos.xyz;
    float view_pow2 = dot(view_direction, view_direction);
    float position_pow2 = dot(ps_in.ObjectSpacePos, ps_in.ObjectSpacePos);
    float pv = -dot(ps_in.ObjectSpacePos, view_direction);
    float m = sqrt(max(pv * pv - view_pow2 * (position_pow2 - r_pow2), 0.0));

    float2 depth = StructureBuffer_Texture.Load(float2(ps_in.Position.xy), 0).zw;

    float t1 = 0.f;
    float t2 = 0.f;
  
    if (depth.x != 0.f)
    {
        float real_camera_z_depth = depth.x + depth.y;
        float t0 = 1.0 + (abs(real_camera_z_depth) / dot(ps_in.CameraZAxisDirObjectSpace, view_direction));
        t1 = clamp((pv - m) / view_pow2, t0, 1.0);
        t2 = clamp((pv + m) / view_pow2, t0, 1.0);
    }
    else
    {
        t1 = clamp((pv - m) / view_pow2, 0.f, 1.0);
        t2 = clamp((pv + m) / view_pow2, 0.f, 1.0);
    }

    float t1_pow2 = t1 * t1;
    float t2_pow2 = t2 * t2;

    float reciprocal_r2 = ps_in.HaloMiscData.y;
    float reciprocal3_r2 = ps_in.HaloMiscData.z;
    float normalizer = ps_in.HaloMiscData.w;

    float B = ((1.f - position_pow2 * reciprocal_r2) * (t2 - t1) + pv * reciprocal_r2 * (t2_pow2 - t1_pow2) -
        view_pow2 * reciprocal3_r2 * (t2 * t2_pow2 - t1 * t1_pow2)) * normalizer;

    return B * B * view_pow2;

}



PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
   

    float4 final_color = float4(0.0, 0.0, 0.0, 0.0);
    float halo_brightness = CalculateHaloFactor(ps_in);

    final_color += float4(ps_in.HaloColor, 1.0) * (halo_brightness);
    ps_out.Color = final_color;

    return ps_out;
}