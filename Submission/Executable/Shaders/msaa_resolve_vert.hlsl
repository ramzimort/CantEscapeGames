


struct PS_IN
{
    float4 Position : SV_Position;
};

PS_IN main(uint vertex_id : SV_VertexID)
{
	// Produce a fullscreen triangle	
    PS_IN ps_in;
    ps_in.Position.x = (vertex_id == 2) ? 3.0 : -1.0;
    ps_in.Position.y = (vertex_id == 0) ? -3.0 : 1.0;
    ps_in.Position.zw = float2(0, 1);
    return ps_in;
}
