#include "Header3D.hlsli"

float4 Pixel3D(Output input) : SV_TARGET
{

	float3 light = normalize(float3(1.f,-1.0,1.f));
	float brightness = dot(-light, input.normal);
	float4 texColor = tex.Sample(smp, input.uv); //テクスチャカラー
	return float4(mul(texColor.x, brightness), mul(texColor.y, brightness), mul(texColor.z, brightness), texColor.w); //return float4(0,0,0,1);
	//return float4(brightness, brightness, brightness, 1); //return float4(0,0,0,1);
}