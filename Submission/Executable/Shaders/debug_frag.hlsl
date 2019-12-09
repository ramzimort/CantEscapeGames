struct PS_IN
{
    float4 Position : SV_Position;
    nointerpolation float3 Color : TEXCOORD0;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
    ps_out.Color = float4(ps_in.Color.rgb, 1.0);
    return ps_out;
}