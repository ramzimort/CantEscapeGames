#include "Shading.h"

struct VS_IN
{
    float3 Position : POSITION;
#if ANIM_MODEL == 1
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : TEXCOORD0;
    int4 BoneIndices : TEXCOORD1;
    float4 BoneWeights : TEXCOORD2;
#endif
};

struct PS_IN
{
    float4 Position : SV_POSITION;
};

cbuffer ObjectUniformBlock : register(b0)
{
    ObjectUniformData ObjectUniformData_Buffer;
};

#if ANIM_MODEL == 1
cbuffer BoneTransformsUniformBlock : register(b1)
{
    BoneTransformsUniformData BoneTransformsUniformData_Buffer;
};
#endif

PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;
    float3 pos_object = float3(0.0, 0.0, 0.0);
#if ANIM_MODEL == 1
    if (vs_in.BoneIndices[0] == -1)
    {
        pos_object = vs_in.Position.xyz;
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
        }
    }
#else
    pos_object = vs_in.Position.xyz;
#endif
    out_data.Position = mul(ObjectUniformData_Buffer.ModelViewProjectionMat, float4(pos_object.xyz, 1.0));
    return out_data;
}