#include "RootSignature.hlsli"
#include "ClothData.hlsli"

StructuredBuffer<inputSpringData> input : register(t0);
RWStructuredBuffer<float> real : register(u0);//RWStructuredBuffer<outputSpringData> output : register(u0);

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothSpring( uint3 DTid : SV_GroupID){
	real[DTid.x] = DTid;
	//real[0] = input[0].pre_id[0];
}