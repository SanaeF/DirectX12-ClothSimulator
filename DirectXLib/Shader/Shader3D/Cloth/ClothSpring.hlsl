#include "ClothProp/RootSignature.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"

RWStructuredBuffer<SpringData> output : register(u0);//RWStructuredBuffer<outputSpringData> output : register(u0);
RWStructuredBuffer<SpringData> input : register(u1);

float3 StructuralSolver(int id) {
	float3 result;
	float3 dx = subtract(input[id].pos, input[id].pre_pos);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = input[id].id0;
		if (ite == 1)id2 = input[id].id1;
		if (ite == 2)id2 = input[id].id2;
		if (ite == 3)id2 = input[id].id3;
		if (id2 == -1)continue;
		float Natulength = distance(input[id].pre_pos, input[id2].pre_pos);//ÇŒÇÀÇÃé©ëRí∑
		float3 f = CalcForce(input[id].pos, input[id2].pos, Natulength, 100);


		//auto size = Natulength - lib::VectorMath::distance(vertex[id].position, vertex[id2].position);
		//if (sqrt(size * size) > Natulength) {
		//	size = lib::VectorMath::distance(pre_vert[id].position, vertex[id].position);;
		//	auto n = lib::VectorMath::subtract(pre_vert[id].position, vertex[id].position);
		//	n = lib::VectorMath::normalize(n);//ê≥ãKâª2
		//	n = lib::VectorMath::scale(n, size * tension_damping);
		//	f = lib::VectorMath::add(f, n);
		//}
		//F=f+damping
		result = add(result, f);
	}
	return result;
}
float3 ShareSolver(int id) {
	float3 result;
	float3 dx = subtract(input[id].pos, input[id].pre_pos);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = input[id].id4;
		if (ite == 1)id2 = input[id].id5;
		if (ite == 2)id2 = input[id].id6;
		if (ite == 3)id2 = input[id].id7;
		if (id2 == -1)continue;
		float Natulength = distance(input[id].pre_pos, input[id2].pre_pos);//ÇŒÇÀÇÃé©ëRí∑
		float3 f = CalcForce(input[id].pos, input[id2].pos, Natulength, 100);


		//auto size = Natulength - lib::VectorMath::distance(vertex[id].position, vertex[id2].position);
		//if (sqrt(size * size) > Natulength) {
		//	size = lib::VectorMath::distance(pre_vert[id].position, vertex[id].position);;
		//	auto n = lib::VectorMath::subtract(pre_vert[id].position, vertex[id].position);
		//	n = lib::VectorMath::normalize(n);//ê≥ãKâª2
		//	n = lib::VectorMath::scale(n, size * tension_damping);
		//	f = lib::VectorMath::add(f, n);
		//}
		//F=f+damping
		result = add(result, f);
	}
	return result;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothSpring( uint3 th_id : SV_GroupID){
	int id = th_id.x;
	output[id] = input[id];
	if (isFixed(input[id].col)) {
		output[id].pos = input[id].pos;
		return;
	}
	float3 f = add(input[id].spring.force, StructuralSolver(id));
	f = add(f, ShareSolver(id));
	output[id].spring.force = f;


	float dt = 0.036;

	float3 v = scale(output[id].spring.force, 1 / (input[id].spring.mass + input[id].spring.mass));
	v = scale(v, dt);
	output[id].spring.velocity = add(input[id].spring.velocity, v);
	v = scale(output[id].spring.velocity, dt);
	output[id].pos = add(input[id].pos, v);
	//óÕÇÉ[ÉçÇ…Ç∑ÇÈ
	output[id].spring.force = float3(0, 0, 0);
}