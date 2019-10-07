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


cbuffer ParticleEmitterStreamOutUniformBlock : register(b0)
{
    ParticleEmitterStreamOutUniformData ParticleEmitterStreamOutUniformData_Buffer;
};


float3 RandomUnitVec3(float offset, ParticleEmitterUniformData emitterData)
{
    // Use game time plus offset to sample random texture.
    float u = (ParticleEmitterStreamOutUniformData_Buffer.GameTime + offset);
   
    float3 v = normalize(RandomOffsets_Textures.SampleLevel(Clamp_To_Edge_Billinear_Sampler, u, 0).xyz);
    
    float radiansYaw = 0.f;
    if (v.x == 0.f && v.z == 0.f)
    {
        radiansYaw = 0.f;
    }
    else
    {
        radiansYaw = atan2(v.z, v.x);
    }
   
    float radiansPitch = atan2(v.y, v.z);
    
    float radiansYawPercentage = radiansYaw / PI_2;
    float radiansPitchPercentage = radiansPitch / PI_2;

    float finalRadiansPitch = radiansPitchPercentage * emitterData.EmitterMiscData.x;
    float finalRadiansYaw = radiansYawPercentage * emitterData.EmitterMiscData.y;
    
    float3 emitterDirection = emitterData.EmitterDirection.xyz;

    float newX = emitterDirection.x;
    float newY = cos(finalRadiansPitch) * emitterDirection.y - sin(finalRadiansPitch) * emitterDirection.z;
    float newZ = sin(finalRadiansPitch) * emitterDirection.y + cos(finalRadiansPitch) * emitterDirection.z;

    float finalX = cos(finalRadiansYaw) * newX + sin(finalRadiansYaw) * newZ;
    float finalY = newY;
    float finalZ = -sin(finalRadiansYaw) * newX + cos(finalRadiansYaw) * newZ;


    return float3(finalX, finalY, finalZ);

    //float u = (ParticleEmitterStreamOutUniformData_Buffer.GameTime + offset);
   
    //float3 v = normalize(RandomOffsets_Textures.SampleLevel(Clamp_To_Edge_Billinear_Sampler, u, 0).xyz);
   
    //float3 truePercentage = v * 0.5 + 0.5;
    
    //float radiansPitchPercentage = truePercentage.x;
    //float radiansYawPercentage = truePercentage.y;
    //float radiansRollPercentage = truePercentage.z;

    //float finalRadiansPitch = radiansPitchPercentage * emitterData.EmitterMiscData.x;
    //float finalRadiansYaw = radiansYawPercentage * emitterData.EmitterMiscData.y;
    
    //float3 emitterDirection = emitterData.EmitterDirection.xyz;

    //float newX = emitterDirection.x;
    //float newY = cos(finalRadiansPitch) * emitterDirection.y - sin(finalRadiansPitch) * emitterDirection.z;
    //float newZ = sin(finalRadiansPitch) * emitterDirection.y + cos(finalRadiansPitch) * emitterDirection.z;

    //float newX2 = cos(finalRadiansYaw) * newX + sin(finalRadiansYaw) * newZ;
    //float newY2 = newY;
    //float newZ2 = -sin(finalRadiansYaw) * newX + cos(finalRadiansYaw) * newZ;


    //return float3(finalX, finalY, finalZ);

    //return normalize(v);
}

[maxvertexcount(MAX_PARTICLE_EMIT_ONCE)]
void main(point GS_IN gs_in[1], inout PointStream<GS_IN> particle_stream)
{
    gs_in[0].ParticleLifetime += ParticleEmitterStreamOutUniformData_Buffer.DeltaTime;

    if (gs_in[0].ParticleType == PARTICLE_TYPE_EMITTER)
    {
        ParticleEmitterUniformData emitter_data = ParticleEmitterStreamOutUniformData_Buffer.EmitterData;
        if (gs_in[0].ParticleLifetime >= emitter_data.EmitterMiscData.z)
        {
            for (uint i = 0; i < emitter_data.EmitterMiscData2.z; ++i)
            {
                float3 newVelocity = RandomUnitVec3(0.0f, emitter_data);
                newVelocity *= 2.f;
                newVelocity *= 2.f;

                GS_IN new_particle;
                new_particle.ParticlePosition = emitter_data.EmitterPosition.xyz;
                new_particle.ParticleVelocity = newVelocity;

                new_particle.ParticleType = PARTICLE_TYPE_FIRE;
                new_particle.ParticleLifetime = 0.f;
                new_particle.ParticleSize = float2(3.f, 3.f);
            
                particle_stream.Append(new_particle);

                gs_in[0].ParticleLifetime = 0.0f;
            }
        }


        if (emitter_data.EmitterMiscData.w != 2.f && (emitter_data.EmitterMiscData.w == 0.f
            || emitter_data.EmitterMiscData2.y < emitter_data.EmitterMiscData2.x))
        {
            particle_stream.Append(gs_in[0]);
        }
    }
    else
    {
        if (gs_in[0].ParticleLifetime <= 1.f)
        {
            particle_stream.Append(gs_in[0]);
        }
    }
}