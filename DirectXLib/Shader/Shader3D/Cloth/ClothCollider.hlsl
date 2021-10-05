#include "ClothProp/CCRootSig.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
struct ColliderData {
	SpringData cloth;
	int area_id;
};
RWStructuredBuffer<ColliderData> output : register(u0);//RWStructuredBuffer<outputSpringData> output : register(u0);
RWStructuredBuffer<ColliderData> input : register(u1);

float3 solver(float3 p1, float3 p2, float hit_dist) {
	float3 n = subtract(p2, p1);
	n = normalize(n);//ê≥ãKâª
	return scale(n, hit_dist);
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothCollider(uint3 th_id : SV_GroupID) {
	int id1 = th_id.x;
	int id2 = th_id.y;
	if (isFixed(input[id1].cloth.col))return;
	output[0].cloth.simulate = true;
	for (int id2 = 0; id2 < 10007; id2++) {
		if (input[id1].area_id == input[id2].area_id) {
			float now_dist = distance(input[id1].cloth.pos, input[id2].cloth.pos);
			float hit_dist = distance(input[id1].cloth.pre_pos, input[id2].cloth.pre_pos) / 2;
			if (now_dist < hit_dist) {
				float f = solver(input[id1].cloth.pos, input[id2].cloth.pos, hit_dist);
				output[id1].cloth.spring.force = add(output[id1].cloth.spring.force, f);
			}
		}
	}
}