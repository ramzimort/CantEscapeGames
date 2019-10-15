#include "Shading.h"

Texture2D<float4> Diffuse_Texture : register(t0);
Texture2D<float4> Normal_Texture : register(t1);
Texture2D<float> Height_Texture : register(t2);

SamplerState Texture_Sampler : register(s0);

struct PS_IN
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLDPOSITION;
    float3 WorldNormal : NORMAL;
    float3 ObjectNormal : OBJECTNORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
    float3 CameraViewTangent : TEXCOORD1;
    float3 CameraSpacePosition : CAMERASPACEPOSITION;
};

struct PS_OUT
{
    float4 WorldNormal : SV_TARGET0;
    float4 Albedo : SV_TARGET1;
    float4 Specular : SV_TARGET2;
    float4 StructureBuffer : SV_TARGET3;
};

cbuffer ObjectUniformBlock : register(b1)
{
    ObjectUniformData ObjectUniformData_Buffer;
};


cbuffer MaterialUniformBlock : register(b2)
{
    MaterialUniformData MaterialUniformData_Buffer;
};


//we transform normal vector from tangent space to object space
float3 CalculateObjectNormalVector(float2 uv, float3 normal, float3 tangent, float3 bitangent, float sigma)
{
    float3 m = Normal_Texture.Sample(Texture_Sampler, uv).rgb;
    float3 n = normalize(normal);
    float3 t = normalize(tangent);
    float3 b = normalize(bitangent);
    return (t * m.x + b * m.y + n * m.z);
}


float2 CalculateParallaxUV(float2 UV, float3 view_tangent)
{
    float height = 1.f - Height_Texture.SampleLevel(Texture_Sampler, UV.xy, 0).r;
    float2 scale = float2(0.0, 0.0);

    scale.x = (height / (8));
    scale.y = -1.f * (height / (8));

    float2 pdir = scale * view_tangent.xy;
    for (int i = 0; i < 4; ++i)
    {
        float3 normal = Normal_Texture.SampleLevel(Texture_Sampler, UV, 0).xyz;
        float cur_height = 1.f - Height_Texture.SampleLevel(Texture_Sampler, UV.xy, 0).r;

        float cur_parallax = normal.z * cur_height;

        UV += pdir * cur_parallax;
    }

    return UV.xy;
}

float4 CalculateStructureBuffer(float object_space_z)
{
    object_space_z = abs(object_space_z);
    float exponent_remainder = asfloat(asuint(object_space_z) & 0xFFFFE000U);

    return float4(ddx(object_space_z), ddy(object_space_z),
        exponent_remainder, object_space_z - exponent_remainder);
}



PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;

    float ambient_strength = 0.1;

    float3 world_normal = ps_in.WorldNormal;
    float2 UV = ps_in.UV;

    int mat_type = (int) (MaterialUniformData_Buffer.MaterialMiscData.w);

    if ((mat_type & MAT_ID_PARALLAX_TEXTURE) != 0)
    {
        UV = CalculateParallaxUV(UV, normalize(ps_in.CameraViewTangent));
    }

    if ((mat_type & MAT_ID_NORMAL_TEXTURE) != 0)
    {
        float3 fetched_normal = CalculateObjectNormalVector(UV, ps_in.ObjectNormal,
            ps_in.Tangent, ps_in.Bitangent, -1.f);

        world_normal = mul(ObjectUniformData_Buffer.NormalMat, float4(fetched_normal, 0.f)).rgb;
    }

    float3 material_diffuse_color = MaterialUniformData_Buffer.DiffuseColor.xyz;

    if ((mat_type & MAT_ID_DIFFUSE_TEXTURE) != 0)
    {
        material_diffuse_color = Diffuse_Texture.Sample(Texture_Sampler, UV).rgb;
    }
    
    //ps_out.WorldPosition = float4(ps_in.WorldPosition, 1.0);
    ps_out.WorldNormal = float4(world_normal, 1.0);
    ps_out.Albedo = float4(material_diffuse_color.rgb, 1.0);
    //ps_out.Albedo = float4(UV.rg, 0.0, 1.0);
    ps_out.Specular = float4(MaterialUniformData_Buffer.SpecularColor.xyz, 1.0);
    ps_out.StructureBuffer = CalculateStructureBuffer(ps_in.CameraSpacePosition.z);

    return ps_out;
}



