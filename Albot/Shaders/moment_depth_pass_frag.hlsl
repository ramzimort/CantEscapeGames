struct PS_IN
{
    float4 Position : SV_POSITION;
};


struct PS_OUT
{
    float4 Color : SV_Target0;
};


PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;

    float z = ps_in.Position.z;
    float z2 = z * z;
    float z3 = z2 * z;
    float z4 = z3 * z;

    ps_out.Color = float4(z, z2, z3, z4);
    return ps_out;
}