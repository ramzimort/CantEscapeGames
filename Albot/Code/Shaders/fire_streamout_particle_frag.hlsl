#include "Shading.h"

Texture2D<float4> Particle_Texture : register(t0);
SamplerState Texture_Sampler : register(s0);

struct PS_IN
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};

struct PS_OUT
{
    float4 Color : SV_TARGET;
};


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;

    //ps_out.Color = float4(1.0, 0.0, 0.0, 1.0);
    ps_out.Color = Particle_Texture.Sample(Texture_Sampler, ps_in.UV).rgba * ps_in.Color;

    return ps_out;
    
}