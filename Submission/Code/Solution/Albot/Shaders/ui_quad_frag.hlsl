#include "Shading.h"

Texture2D<float4> UITexture : register(t0);

struct PS_IN
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};


SamplerState Texture_Sampler : register(s0);



struct PS_OUT
{
    float4 Color : SV_Target0;
};


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
    ps_out.Color = UITexture.Sample(Texture_Sampler, ps_in.UV).rgba;
    return ps_out;
}
