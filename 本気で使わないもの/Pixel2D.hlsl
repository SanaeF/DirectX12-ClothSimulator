#include "Header2D.hlsli"

float4 Pixel2D(Output input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv));
}