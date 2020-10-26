#include "Header2D.hlsli"

Output Vertex2D(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;
	output.svpos = mul(mat, pos);
	output.uv = uv;

	return output;
}