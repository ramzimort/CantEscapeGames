#include "Shading.h"

StructuredBuffer<LineVertexUniformData> LineVertexUniform_Buffer : register(t0);

cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};

struct VS_IN
{
    float3 Position : POSITION;
    uint VertexID : SV_VertexID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    nointerpolation float3 Color : TEXCOORD0;
};


PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;

    uint buffer_id = vs_in.VertexID / 2;
    
    float4 line_color = LineVertexUniform_Buffer[buffer_id].Color.rgba;

    ps_in.Position = mul(CameraUniformData_Buffer.ViewProjectionMat, float4(vs_in.Position.xyz, 1.0));
    ps_in.Color = line_color.rgb;

    return ps_in;
}