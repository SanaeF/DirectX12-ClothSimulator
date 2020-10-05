#include "HeaderPolygon.hlsli"

float4 ps(Output input) : SV_Target
{

	return float4(input.uv,1,1);
}