#include "Shading.h"

Texture2D<float4> Diffuse_Texture : register(t0);
Texture2D<float4> Normal_Texture : register(t1);
Texture2D<float> Height_Texture : register(t2);
Texture2D<float> Specular_Texture : register(t3);
Texture2D<float> Metallic_Texture : register(t4);
Texture2D<float> Roughness_Texture : register(t5);

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
    float4 MaterialProperty : SV_TARGET3;
    float4 StructureBuffer : SV_TARGET4;
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
    m = normalize(m * 2.0 - 1.0);
    float3 n = normalize(normal);
    float3 t = normalize(tangent);
    float3 b = normalize(bitangent) * sigma;
    return normalize((t * m.x + b * m.y + n * m.z));
}


float2 CalculateParallaxUV(float2 UV, float3 view_tangent)
{
    float height = Height_Texture.Sample(Texture_Sampler, UV.xy).r;
    float2 scale = float2(0.0, 0.0);

    scale.x = (height / (16.0));
    scale.y = -1.f * (height / (16.0));

    float2 pdir = scale * view_tangent.xy;
    for (int i = 0; i < 4; ++i)
    {
        float3 normal = Normal_Texture.Sample(Texture_Sampler, UV).xyz;
        float cur_height = Height_Texture.Sample(Texture_Sampler, UV.xy).r;

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

    float material_roughness = MaterialUniformData_Buffer.MaterialMiscData2.x;
    float material_metallic = MaterialUniformData_Buffer.MaterialMiscData2.y;
    float3 material_specular_color = MaterialUniformData_Buffer.SpecularColor.xyz;

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
    if ((mat_type & MAT_ID_SPECULAR_TEXUTERE) != 0)
    {
        material_specular_color = material_diffuse_color * Specular_Texture.Sample(Texture_Sampler, UV).r;
    }
    if ((mat_type & MAT_ID_METALLIC_TEXTURE) != 0)
    {
        material_metallic = Metallic_Texture.Sample(Texture_Sampler, UV).r;
    }
    if ((mat_type & MAT_ID_ROUGHNESS_TEXTURE) != 0)
    {
        material_roughness = Roughness_Texture.Sample(Texture_Sampler, UV).r;
    }
    
    ps_out.WorldNormal = float4(world_normal, 1.0);
    ps_out.Albedo = float4(material_diffuse_color.rgb, 1.0);
    ps_out.MaterialProperty.x = material_roughness;
    ps_out.MaterialProperty.y = material_metallic;
    ps_out.MaterialProperty.zw = float2(0.f, 0.f);
    ps_out.Specular = float4(MaterialUniformData_Buffer.SpecularColor.xyz, 1.0);
    ps_out.StructureBuffer = CalculateStructureBuffer(ps_in.CameraSpacePosition.z);

    return ps_out;
}



