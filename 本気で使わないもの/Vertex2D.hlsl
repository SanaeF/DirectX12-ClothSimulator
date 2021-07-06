#include "Header2D.hlsli"

Output Vertex2D(float4 pos : POSITION, float2 uv : TEXCOORD, uint id : SV_InstanceID)//, matrix mat:MATRIX
{
	Output output;
    //output.mat = mat;
    output.svpos = mul(mMat, pos); //instData[id].InstMat
	output.uv = uv;
	//output.id = id;
    return output;
}
