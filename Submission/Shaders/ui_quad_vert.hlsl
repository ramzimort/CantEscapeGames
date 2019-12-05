#include "Shading.h"

struct VS_IN
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

cbuffer UIObjectUniformBlock : register(b0)
{
    UIObjectUniformData UIObjectUniformData_Buffer;
};

PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;
    out_data.Position = mul(UIObjectUniformData_Buffer.ModelViewProjectionMat, float4(vs_in.Position.xyz, 1.0));
    //Quick dirty hack
    //somehow the UV is inverted
    //which is why i am doing it this way
    float2 finalUV = vs_in.UV;
    finalUV.y = finalUV.y == 1.0 ? 0.0 : 1.0;
    out_data.UV = finalUV;
    return out_data;
}