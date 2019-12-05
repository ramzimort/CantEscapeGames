#include "Shading.h"

struct GS_IN
{
    float3 WorldPosition : POSITION;
    float4 Color : COLOR;
    float2 Size : PARTICLESIZE;
    uint ParticleType : PARTICLETYPE;
};


struct PS_IN
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};


//cbuffer GlobalFixedConstants
//{
//    float2 gQuadUV[4] =
//    {
//        float2(0.0f, 1.0f),
//		float2(1.0f, 1.0f),
//		float2(0.0f, 0.0f),
//		float2(1.0f, 0.0f)
//    };
//};


cbuffer CameraUniformData_CB : register(b0)
{
    CameraUniformData CameraUniformData_Buffer;
};


[maxvertexcount(4)]
void main(point GS_IN gs_in[1], inout TriangleStream<PS_IN> triangle_stream)
{

    float2 gQuadUV[4] =
    {
        float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
    };

    if(gs_in[0].ParticleType != PARTICLE_TYPE_EMITTER)
    {
        float3 camera_pos = CameraUniformData_Buffer.CameraPosition.xyz;
        
        float3 particle_pos_to_camera_dir = normalize(camera_pos - gs_in[0].WorldPosition.xyz);
        float3 right_dir = normalize(cross(particle_pos_to_camera_dir, float3(0.0, 1.0, 0.0)));
        float3 up_dir = normalize(cross(right_dir, particle_pos_to_camera_dir));


        //right_dir = float3(1.f, 0.0f, 0.0f);
        //up_dir = float3(0.f, 1.f, 0.f);
        //float3 right_dir = normalize(cross(float3(0.0, 1.0, 0.0), particle_pos_to_camera_dir));
        //float3 up_dir = normalize(cross(particle_pos_to_camera_dir, right_dir));

        float2 half_size = 0.5f * gs_in[0].Size;
        
        float3 vertices[4];

        vertices[0] = float3(gs_in[0].WorldPosition.xyz + 
            (half_size.x * right_dir) - (half_size.y * up_dir));

        
        vertices[1] = float3(gs_in[0].WorldPosition.xyz +
            (half_size.x * right_dir) + (half_size.y * up_dir));

        vertices[2] = float3(gs_in[0].WorldPosition.xyz -
            (half_size.x * right_dir) - (half_size.y * up_dir));

        vertices[3] = float3(gs_in[0].WorldPosition.xyz -
            (half_size.x * right_dir) + (half_size.y * up_dir));

        for (int i = 0; i < 4; ++i)
        {
            PS_IN new_tri_point;
            new_tri_point.Position = mul(CameraUniformData_Buffer.ViewProjectionMat, float4(vertices[i], 1.0));
            //new_tri_point.Position = float4(vertices[i].xyz, 1.0);
            new_tri_point.Color = gs_in[0].Color;
            new_tri_point.UV = gQuadUV[i];
            triangle_stream.Append(new_tri_point);
        }

    }
}