

Texture2D<float4> Input_Texture : register(t0);

struct PS_IN
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

struct PS_OUT
{
    float4 Color : SV_Target0;
};


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
    ps_out.Color = Input_Texture.Load(uint3(ps_in.Position.xy, 0)).rgba;
    return ps_out;
}
