#include "Header3D.hlsli"

Output Vertex3D(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneno : BONE_NO, min16uint weight : WEIGHT)//, matrix mat:MATRIX
{
	Output output;
	pos = mul(world, pos);
	output.svpos = mul(mul(proj, view), pos);
	output.pos = mul(view, pos);
	normal.w = 0;
	output.normal = mul(world, normal);
	output.vnormal = mul(view, output.normal);
	output.uv = uv;
	output.ray = normalize(pos.xyz - mul(view, eye));
	return output;
}
