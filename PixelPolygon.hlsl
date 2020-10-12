#include "HeaderPolygon.hlsli"

float4 ps(Output input) : SV_Target
{

	return tex.Sample(smp, input.uv);
}