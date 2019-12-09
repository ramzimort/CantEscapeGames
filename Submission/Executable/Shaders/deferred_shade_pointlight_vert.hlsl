#include "Shading.h"


StructuredBuffer<ConstantPointLightData> ConstantLightDataBuffer : register(t0);

struct VS_IN
{
    float3 Position : POSITION;
    uint InstanceID : SV_InstanceID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    nointerpolation float3 LightPosition : TEXCOORD0;
    nointerpolation float3 LightColor : TEXCOORD1;
    nointerpolation float4 LightMiscData : TEXCOORD2;
};


cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;

    ConstantPointLightData cur_point_light_data = ConstantLightDataBuffer[vs_in.InstanceID];

    float3 final_pos = (vs_in.Position * cur_point_light_data.LightMiscData.x) + cur_point_light_data.LightPosition.xyz;

    ps_in.Position = mul(CameraUniformData_Buffer.ViewProjectionMat, float4(final_pos.xyz, 1.0));
    ps_in.LightPosition = cur_point_light_data.LightPosition.xyz;
    ps_in.LightColor = cur_point_light_data.LightColor.rgb;
    ps_in.LightMiscData = cur_point_light_data.LightMiscData.xyzw;

    return ps_in;
}