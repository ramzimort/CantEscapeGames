#include "Shading.h"

Texture1D<float4> RandomOffsets_Textures : register(t0);
SamplerState Clamp_To_Edge_Billinear_Sampler : register(s0);

struct GS_IN
{
    float3 ParticlePosition : POSITION;
    float3 ParticleVelocity : TEXCOORD0;
    float2 ParticleSize : TEXCOORD1;
    float ParticleLifetime : TEXCOORD2;
    uint ParticleType : TEXCOORD3;
};


cbuffer ParticleStreamOutUniformBlock : register(b0)
{
    ParticleStreamOutUniformData ParticleStreamOutUniformData_Buffer;
};


float3 RandomUnitVec3(float offset)
{
    // Use game time plus offset to sample random texture.
    float u = (ParticleStreamOutUniformData_Buffer.GameTime + offset);
   
    float3 v = RandomOffsets_Textures.SampleLevel(Clamp_To_Edge_Billinear_Sampler, u, 0).xyz;
    
    return normalize(v);
}

[maxvertexcount(2)]
void main(point GS_IN gs_in[1], inout PointStream<GS_IN> particle_stream)
{
    //GS_IN output_particle;
    //output_particle.ParticlePosition = gs_in[0].ParticlePosition;
    //output_particle.ParticleVelocity = gs_in[0].ParticleVelocity;
    //output_particle.ParticleSize = gs_in[0].ParticleSize;
    //output_particle.ParticleLifetime = gs_in[0].ParticleLifetime;
    //output_particle.ParticleType = gs_in[0].ParticleType;

    //particle_stream.Append(output_particle);
    gs_in[0].ParticleLifetime += ParticleStreamOutUniformData_Buffer.DeltaTime;
    
    if (gs_in[0].ParticleType == PARTICLE_TYPE_EMITTER)
    {
        //if (gs_in[0].ParticleLifetime >= 0.0005f)
        //{
            float3 newVelocity = RandomUnitVec3(0.0f);
            newVelocity *= 2.f;
            newVelocity *= 2.f;

            GS_IN new_particle;
            new_particle.ParticlePosition = ParticleStreamOutUniformData_Buffer.EmitterPosition.xyz;
            new_particle.ParticleVelocity = newVelocity;

            new_particle.ParticleType = PARTICLE_TYPE_FIRE;
            new_particle.ParticleLifetime = 0.f;
            new_particle.ParticleSize = float2(3.f, 3.f);
            
            particle_stream.Append(new_particle);

            gs_in[0].ParticleLifetime = 0.0f;
        //}
        particle_stream.Append(gs_in[0]);
    }
    else
    {
        if (gs_in[0].ParticleLifetime <= 1.f)
        {
            particle_stream.Append(gs_in[0]);
        }
    }
}