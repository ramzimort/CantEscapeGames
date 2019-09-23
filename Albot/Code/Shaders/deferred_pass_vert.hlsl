#include "Shading.h"

struct VS_IN
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
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

void CalculateTangentSpaceV(float3 vertex_pos, float3 obj_tangent, float3 obj_bitangent, float3 obj_normal, out float3 view_tan)
{
    float3 camera_obj_pos = mul(ObjectUniformData_Buffer.InvModelMat, CameraUniformData_Buffer.CameraPosition).xyz;
    float3 v = camera_obj_pos - vertex_pos;
    view_tan = float3(dot(obj_tangent, v), dot(obj_bitangent, v), dot(obj_normal, v));
}


PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;
    float3 view_tangent = float3(0.0, 0.0, 0.0);
    CalculateTangentSpaceV(vs_in.Position.xyz, vs_in.Tangent, vs_in.Bitangent, vs_in.Normal, view_tangent);
    out_data.Position = mul(ObjectUniformData_Buffer.ModelViewProjectionMat, float4(vs_in.Position, 1.0f)).xyzw;
    out_data.WorldPosition = mul(ObjectUniformData_Buffer.ModelMat, float4(vs_in.Position, 1.0f)).xyz;
    out_data.WorldNormal = normalize(mul(ObjectUniformData_Buffer.NormalMat, float4(vs_in.Normal, 0.0f)).xyz);
    out_data.ObjectNormal = vs_in.Normal;
    out_data.Tangent = vs_in.Tangent;
    out_data.Bitangent = vs_in.Bitangent;
    //TODO
    out_data.UV = vs_in.UV * MaterialMiscData.xy;
    out_data.CameraViewTangent = view_tangent;
    return out_data;
}