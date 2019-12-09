
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
    float3 skybox_color = Skybox_Texture.Sample(trillinear_sampler, ps_in.ObjectPosition).rgb;
    skybox_color = skybox_color / (skybox_color + float3(1.0, 1.0, 1.0));
    skybox_color = pow(skybox_color, float3(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f));

    ps_out.Color = float4(skybox_color.rgb, 1.0);
    return ps_out;
}




