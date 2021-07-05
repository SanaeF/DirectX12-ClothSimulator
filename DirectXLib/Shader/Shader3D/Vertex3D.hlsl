#include "Header3D.hlsli"

Output Vertex3D(float4 pos : POSITION, float4 normal : NORMAL, float3 uv : TEXCOORD, min16uint2 boneno : BONE_NO, min16uint weight : WEIGHT)//, matrix mat:MATRIX
{
	Output output;
	//output.mat = mat;
	output.svpos = mul(mul(viewproj, world), pos);
	//output.svpos.x = mul(mul(viewproj, world), pos.x);
	//output.svpos.y = mul(mul(viewproj, world), pos.y);
	//output.svpos.z = mul(mul(viewproj, world), pos.z);
	normal.w = 0;
	output.normal = mul(world, normal);
	output.uv = uv;
	//output.id = id;
	return output;
}
