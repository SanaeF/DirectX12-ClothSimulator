#include "Header3D.hlsli"

float4 Pixel3D(Output input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));
	float brightness = dot(-light, input.normal);
	return float4(brightness, brightness, brightness, 1); //return float4(0,0,0,1);
}