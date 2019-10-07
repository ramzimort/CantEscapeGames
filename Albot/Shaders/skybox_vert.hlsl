
struct VS_IN
{
    //float3 Position : POSITION;
    float4 Position : POSITION;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float3 ObjectPosition : OBJECTPOSITION;
};

cbuffer SkyboxUniformDataBuffer : register(b0)
{
    float4x4 ModelViewProjectionMat;
};

PS_IN main(VS_IN vs_in)
{
    PS_IN out_data;
    out_data.Position = mul(ModelViewProjectionMat, vs_in.Position).xyww;
    out_data.ObjectPosition = (vs_in.Position.xyz);

    return out_data;
}