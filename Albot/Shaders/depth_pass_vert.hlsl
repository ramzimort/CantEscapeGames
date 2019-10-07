#include "Shading.h"

struct VS_IN
{
    float3 Position : POSITION;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
};

cbuffer ObjectUniformBlock : register(b0)
{
    ObjectUniformData ObjectUniformData_Buffer;
};

PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;
    out_data.Position = mul(ObjectUniformData_Buffer.ModelViewProjectionMat, float4(vs_in.Position, 1.0));
    return out_data;
}