#include "Shading.h"
#include "PBR.hlsl"

RWTexture2D<float2> BRDFLookupTexture : register(u0);

cbuffer ProcessBRDFLookupTextureUniformData_CB : register(b0)
{
    ProcessBRDFLookupTextureUniformData ProcessBRDFLookupTextureUniformData_Buffer;
};


float2 IntegrateBRDF(float N_dot_V, float roughness)
{
    float3 V_term = float3(sqrt(1.0 - N_dot_V * N_dot_V), 0.0, N_dot_V);
    float A_term = 0.0;
    float B_term = 0.0;

    float3 N_term = float3(0.0, 0.0, 1.0);
    const uint sample_count = 1024u;

    for (uint i = 0u; i < sample_count; ++i)
    {
        float2 Xi = HammersleySequence(i, sample_count);
        float3 H_term = ImportanceSampleGGX(Xi, N_term, roughness);
        //reflection on halfway vector term based on the sphere direction
        float3 L_term = normalize(2.0 * dot(V_term, H_term) * H_term - V_term);
        float N_dot_L = max(L_term.z, 0.0);
       
        if (N_dot_L > 0.0)
        {
            float N_dot_H = max(H_term.z, 0.0);
            float V_dot_H = max(dot(V_term, H_term), 0.0);

            float G_term = GeometrySmithIBL(N_term, L_term, V_term, roughness);
            float G_Vis = (G_term * V_dot_H) / (N_dot_H * N_dot_V);
            float Fc = pow(1.0 - V_dot_H, 5.0);

            A_term += (1.0 - Fc) * G_Vis;
            B_term += Fc * G_Vis;
        }
    }

    A_term /= float(sample_count);
    B_term /= float(sample_count);

    return float2(A_term, B_term);
}

[numthreads(PROCESS_BRDF_LUT_WORKGROUP_THREAD_ID_X, PROCESS_BRDF_LUT_WORKGROUP_THREAD_ID_Y, PROCESS_BRDF_LUT_WORKGROUP_THREAD_ID_Z)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    float2 final_uv = float2(float(DispatchThreadID.x) + 0.5, float(DispatchThreadID.y) + 0.5 )
        / ProcessBRDFLookupTextureUniformData_Buffer.BRDFLUTTextureSize.xy;

    BRDFLookupTexture[DispatchThreadID.xy] = IntegrateBRDF(final_uv.x, final_uv.y);
}