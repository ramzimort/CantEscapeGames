#include "Shading.h"

StructuredBuffer<DebugInstanceUniformData> DebugInstanceUniform_Buffer : register(t0);

struct VS_IN
{
    float3 Position : POSITION;
    uint InstanceID : SV_InstanceID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    nointerpolation float3 Color : TEXCOORD0;
};


PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;

    DebugInstanceUniformData cur_debug_data = DebugInstanceUniform_Buffer[vs_in.InstanceID];

    ps_in.Position = mul(cur_debug_data.ModelViewProjectionMat, float4(vs_in.Position.xyz, 1.0));
    ps_in.Color = cur_debug_data.Color.rgb;

    return ps_in;
}