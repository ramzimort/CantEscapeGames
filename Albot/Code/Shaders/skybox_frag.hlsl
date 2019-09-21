
TextureCube<float4> Skybox_Texture : register(t0);

SamplerState trillinear_sampler : register(s0);


struct PS_IN
{
    float4 Position : SV_POSITION;
    float3 ObjectPosition : OBJECTPOSITION;
};

struct PS_OUT
{
    float4 Color : SV_Target0;
};

PS_OUT main(PS_IN ps_in)
{
    PS_OUT ps_out;
    ps_out.Color = Skybox_Texture.Sample(trillinear_sampler, ps_in.ObjectPosition).rgba;
    //ps_out.Color = float4(ps_in.Position.xyz, 1.0);
    return ps_out;
}




