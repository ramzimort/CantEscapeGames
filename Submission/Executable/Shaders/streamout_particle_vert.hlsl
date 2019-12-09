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
    float3 ParticlePosition : POSITION;
    float3 ParticleVelocity : TEXCOORD0;
    float2 ParticleSize : TEXCOORD1;
    float ParticleLifetime : TEXCOORD2;
    uint ParticleType : TEXCOORD3;
};


GS_IN main(VS_IN vs_in)
{
    GS_IN gs_in;

    gs_in.ParticlePosition = vs_in.ParticlePosition;
    gs_in.ParticleVelocity = vs_in.ParticleVelocity;
    gs_in.ParticleSize = vs_in.ParticleSize;
    gs_in.ParticleLifetime = vs_in.ParticleLifetime;
    gs_in.ParticleType = vs_in.ParticleType;

    return gs_in;
}
