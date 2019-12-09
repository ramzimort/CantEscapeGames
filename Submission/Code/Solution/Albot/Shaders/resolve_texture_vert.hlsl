#include "Shading.h"


cbuffer ResolveTextureUniform_CB : register(b0)
{
    ResolveRendererInstancesUniformData ResolveRendererInstancesUniformData_Buffer;
};



struct VS_IN
{
    uint VertexID : SV_VertexID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;
    float2 pos = float2(-1.f, 1.f);
    float2 UV = float2(0.f, 0.f);
    
    
   
    if (vs_in.VertexID == 1)
    {
        pos = float2(-1.0, ResolveRendererInstancesUniformData_Buffer.Scale.y);
        UV = float2(0.f, 1.f);
    }
    else if (vs_in.VertexID == 2 || 
        vs_in.VertexID == 3)
    {
        pos = float2(ResolveRendererInstancesUniformData_Buffer.Scale.xy);
        UV = float2(1.f, 1.f);
    }
    
    else if (vs_in.VertexID == 4)
    {
        pos = float2(ResolveRendererInstancesUniformData_Buffer.Scale.x, 1.0);
        UV = float2(1.f, 0.f);
    }

    float2 final_pos = (pos.xy) + ResolveRendererInstancesUniformData_Buffer.Translation.xy;

    ps_in.Position = float4(final_pos.xy, 0.f, 1.f);
    ps_in.UV = UV;

    return ps_in;
}