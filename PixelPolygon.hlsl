float4 ps(Output input) : SV_Tatget
{

	return float4(input.uv,1,1);
}