#include "Shading.h"

struct VS_IN
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
#if ANIM_MODEL == 1
    int4 BoneIndices : TEXCOORD1;
    float4 BoneWeights : TEXCOORD2;
#endif
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLDPOSITION;
    float3 WorldNormal : NORMAL;
    float3 ObjectNormal : OBJECTNORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
    //not normalized
    float3 CameraViewTangent : TEXCOORD1;
    float3 CameraSpacePosition : CAMERASPACEPOSITION;
};


cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


cbuffer ObjectUniformBlock : register(b1)
{
    ObjectUniformData ObjectUniformData_Buffer;
};


cbuffer MaterialUniformBlock : register(b2)
{
    float4 DiffuseColor;
    float4 SpecularColor;
    float4 MaterialMiscData;
};

#if ANIM_MODEL == 1
cbuffer BoneTransformsUniformBlock : register(b3)
{
    BoneTransformsUniformData BoneTransformsUniformData_Buffer;
};
#endif

void CalculateTangentSpaceV(float3 vertex_pos, float3 obj_tangent, float3 obj_bitangent, float3 obj_normal, out float3 view_tan)
{
    float3 camera_obj_pos = mul(ObjectUniformData_Buffer.InvModelMat, CameraUniformData_Buffer.CameraPosition).xyz;
    float3 v = camera_obj_pos - vertex_pos;
    view_tan = float3(dot(obj_tangent, v), dot(obj_bitangent, v), dot(obj_normal, v));
}


PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;

#if ANIM_MODEL == 1
    float3 pos_object = float3(0.0, 0.0, 0.0);
    float3 normal_object = float3(0.0, 0.0, 0.0);
    float3 tangent_object = float3(0.0, 0.0, 0.0);
    float3 bitangent_object = float3(0.0, 0.0, 0.0);

    if (vs_in.BoneIndices[0] == -1)
    {
        pos_object = vs_in.Position.xyz;
        normal_object = vs_in.Normal.xyz;
        tangent_object = vs_in.Tangent.xyz;
        bitangent_object = vs_in.Bitangent.xyz;
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            if (vs_in.BoneIndices[i] == -1)
            {
                break;
            }
            pos_object += vs_in.BoneWeights[i] * mul(BoneTransformsUniformData_Buffer.
                BoneTransforms[vs_in.BoneIndices[i]], float4(vs_in.Position.xyz, 1.0)).xyz;

            normal_object += vs_in.BoneWeights[i] * mul(BoneTransformsUniformData_Buffer.
                BoneTransforms[vs_in.BoneIndices[i]], float4(vs_in.Normal.xyz, 0.0)).xyz;

            tangent_object += vs_in.BoneWeights[i] * mul(BoneTransformsUniformData_Buffer.
                BoneTransforms[vs_in.BoneIndices[i]], float4(vs_in.Tangent.xyz, 0.0)).xyz;

            bitangent_object += vs_in.BoneWeights[i] * mul(BoneTransformsUniformData_Buffer.
                BoneTransforms[vs_in.BoneIndices[i]], float4(vs_in.Bitangent.xyz, 0.0)).xyz;
        }
    }
    normal_object = normalize(normal_object);
    tangent_object = normalize(tangent_object);
    bitangent_object = normalize(bitangent_object);
#else
    float3 pos_object = float3(vs_in.Position.xyz);
    float3 normal_object = float3(vs_in.Normal.xyz);
    float3 tangent_object = float3(vs_in.Tangent.xyz);
    float3 bitangent_object = float3(vs_in.Bitangent.xyz);
#endif

    float3 view_tangent = float3(0.0, 0.0, 0.0);
    CalculateTangentSpaceV(pos_object.xyz, vs_in.Tangent, vs_in.Bitangent, vs_in.Normal, view_tangent);
    out_data.Position = mul(ObjectUniformData_Buffer.ModelViewProjectionMat, float4(pos_object, 1.0f)).xyzw;
    out_data.WorldPosition = mul(ObjectUniformData_Buffer.ModelMat, float4(pos_object, 1.0f)).xyz;
    out_data.WorldNormal = normalize(mul(ObjectUniformData_Buffer.NormalMat, float4(normal_object, 0.0f)).xyz);
    out_data.ObjectNormal = normal_object;
    out_data.Tangent = tangent_object;
    out_data.Bitangent = bitangent_object;


    float4 camera_space_pos = mul(CameraUniformData_Buffer.ViewMat, float4(out_data.WorldPosition, 1.0)).xyzw;
    camera_space_pos.z /= camera_space_pos.w;
    out_data.CameraSpacePosition = camera_space_pos.xyz;


    out_data.UV = vs_in.UV * MaterialMiscData.xy;
    out_data.CameraViewTangent = view_tangent;
    return out_data;
}