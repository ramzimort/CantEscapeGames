#include "Shading.h"

StructuredBuffer<ConstantHaloEffectData> ConstantHaloEffectBuffer : register(t0);

struct VS_IN
{
    float3 Position : POSITION;
    uint InstanceID : SV_InstanceID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    float3 ObjectSpacePos : OBJECT_SPACE_POS;
    float3 CameraPosObjectSpace : CAMERA_OBJ_SPACE;
    float3 CameraZAxisDirObjectSpace : CAMERA_DIR_SPACE;
    nointerpolation float3 HaloColor : HALO_COLOR;
    nointerpolation float4 HaloMiscData : HALO_MISC_DATA;
};


cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;

    ConstantHaloEffectData curHaloEffectData = ConstantHaloEffectBuffer[vs_in.InstanceID];

    float haloScale = curHaloEffectData.HaloMiscData2.x;

    float3 final_pos = (vs_in.Position * haloScale) + curHaloEffectData.HaloPosition.xyz;

    ps_in.CameraPosObjectSpace = (CameraUniformData_Buffer.CameraPosition.xyz - curHaloEffectData.HaloPosition.xyz);
    ps_in.CameraZAxisDirObjectSpace = (CameraUniformData_Buffer.InvViewMat._13_23_33.xyz * float3(-1.f, -1.f, -1.f));

    ps_in.ObjectSpacePos = vs_in.Position.xyz * haloScale;
    ps_in.Position = mul(CameraUniformData_Buffer.ViewProjectionMat, float4(final_pos.xyz, 1.0));
    ps_in.HaloColor = curHaloEffectData.HaloColor.rgb;
    ps_in.HaloMiscData = curHaloEffectData.HaloMiscData.xyzw;

    return ps_in;
}
