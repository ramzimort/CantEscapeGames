#include "Shading.h"

struct VS_IN
{
    float3 ParticlePosition : POSITION;
    float3 ParticleVelocity : TEXCOORD0;
    float2 ParticleSize : TEXCOORD1;
    float ParticleLifetime : TEXCOORD2;
    uint ParticleType : TEXCOORD3;
};


struct GS_IN
{
    float3 WorldPosition : POSITION;
    float4 Color : COLOR;
    float2 Size : PARTICLESIZE;
    uint ParticleType : PARTICLETYPE;
};


cbuffer ParticleConstantValue
{
    float3 gAcceleration = { -1.0f, -9.8f, 0.0f };
};

GS_IN main(VS_IN vs_in)
{
    GS_IN gs_in;

    float3 final_pos = 0.5f * vs_in.ParticleLifetime * vs_in.ParticleLifetime * gAcceleration + 
        vs_in.ParticleLifetime * vs_in.ParticleVelocity + vs_in.ParticlePosition;

    //final_pos = vs_in.ParticlePosition;

    float opacity = 1.f - smoothstep(0.f, 1.f, vs_in.ParticleLifetime);
    float4 final_color = float4(1.0, 1.0, 1.0, opacity);


    gs_in.WorldPosition = final_pos.xyz;
    gs_in.Color = final_color;
    gs_in.Size = vs_in.ParticleSize.xy;
    gs_in.ParticleType = vs_in.ParticleType;

    return gs_in;
}