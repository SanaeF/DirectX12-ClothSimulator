#include "Header2D.hlsli"

Output Vertex2D(float4 pos : POSITION, float2 uv : TEXCOORD, uint id : SV_InstanceID)
{
	Output output;
	output.svpos = mul(mMat[id], pos);//mul(id, mMat)
	output.uv = uv;
	//output.id = id;
	return output;
}