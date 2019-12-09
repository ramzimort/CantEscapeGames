
struct VS_IN
{
    uint VertexID : SV_VertexID;
};

struct PS_IN
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

PS_IN main(VS_IN vs_in)
{
    PS_IN ps_in;
    float2 pos = float2(-1.f, 1.f);
    float2 UV = float2(0.f, 0.f);
    
    if (vs_in.VertexID == 1)
    {
        pos = float2(-1.0, -1.0);
        UV = float2(0.f, 1.f);
    }
    else if (vs_in.VertexID == 2)
    {
        pos = float2(1.0, -1.0);
        UV = float2(1.f, 1.f);
    }
    else if (vs_in.VertexID == 3)
    {
        pos = float2(1.0, -1.0);
        UV = float2(1.f, 1.f);
    }
    else if (vs_in.VertexID == 4)
    {
        pos = float2(1.0, 1.0);
        UV = float2(1.f, 0.f);
    }
    else if (vs_in.VertexID == 5)
    {
        pos = float2(-1.0, 1.0);
        UV = float2(0.f, 0.f);
    }

    ps_in.Position = float4(pos.xy, 0.f, 1.f);
    ps_in.UV = UV;

    return ps_in;
}