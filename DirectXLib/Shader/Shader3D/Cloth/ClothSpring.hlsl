#include "ClothProp/CSRootSig.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"

RWStructuredBuffer<SpringData> output : register(u0);//RWStructuredBuffer<outputSpringData> output : register(u0);
RWStructuredBuffer<SpringData> input : register(u1);
RWStructuredBuffer<ModelParamater> param :register(u2);

float3 CompressionSolver(int id) {
	int tension = 15;
	int tension_damping = 5;
	float3 result = float3(0.f, 0.f, 0.f);
	return result;
}
float3 StructuralSolver(int id) {
	int tension = 15;
	int tension_damping = 5;
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
		float3 f = CalcForce(input[id].pos, input[id2].pos, Natulength, tension, tension_damping);

		//float size = Natulength - distance(input[id].pos, input[id2].pos);
		//if (sqrt(size * size) > Natulength) {
		//	size = distance(input[id].pre_pos, input[id].pos);
		//	float3 n = subtract(input[id].pre_pos, input[id].pos);
		//	n = normalize(n);//ê≥ãKâª2
		//	n = scale(n, size * tension_damping);
		//	f = add(f, n);
		//}
		//F=f+damping
		result = add(result, f);
	}
	return result;
}
float3 ShareSolver(int id) {
	int shear = 15;
	int shear_damping = 5;
	float3 result = float3(0.f, 0.f, 0.f);
	float3 dx = subtract(input[id].pos, input[id].pre_pos);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = input[id].id4;
		if (ite == 1)id2 = input[id].id5;
		if (ite == 2)id2 = input[id].id6;
		if (ite == 3)id2 = input[id].id7;
		if (id2 == -1)continue;
		float Natulength = distance(input[id].pre_pos, input[id2].pre_pos);//ÇŒÇÀÇÃé©ëRí∑
		float3 f = CalcForce(input[id].pos, input[id2].pos, Natulength, shear, shear_damping);

		//float size = Natulength - distance(input[id].pos, input[id2].pos);
		//if (sqrt(size * size) > Natulength) {
		//	size = distance(input[id].pre_pos, input[id].pos);
		//	float3 n = subtract(input[id].pre_pos, input[id].pos);
		//	n = normalize(n);//ê≥ãKâª2
		//	n = scale(n, size * shear_damping);
		//	f = add(f, n);
		//}
		//F=f+damping
		result = add(result, f);
	}
	return result;
}
float3 BendingSolver(int id) {
	int tension = 20;
	int tension_damping = 5;
	float3 result = float3(0.f, 0.f, 0.f);
	float3 dx = subtract(input[id].pos, input[id].pre_pos);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = input[id].id8;
		if (ite == 1)id2 = input[id].id9;
		if (ite == 2)id2 = input[id].id10;
		if (ite == 3)id2 = input[id].id11;
		if (id2 == -1)continue;
		float Natulength = distance(input[id].pre_pos, input[id2].pre_pos);//ÇŒÇÀÇÃé©ëRí∑
		float3 f = CalcForce(input[id].pos, input[id2].pos, Natulength, tension, tension_damping);

		//float size = Natulength - distance(input[id].pos, input[id2].pos);
		//if (sqrt(size * size) > Natulength) {
		//	size = distance(input[id].pre_pos, input[id].pos);
		//	float3 n = subtract(input[id].pre_pos, input[id].pos);
		//	n = normalize(n);//ê≥ãKâª2
		//	n = scale(n, size * tension_damping);
		//	f = add(f, n);
		//}

		//F=f+damping
		result = add(result, f);
	}
	return result;
}

void firstSetting(int id) {
	output[0].simulate = true;

	if (id == 0) {
		param[0].max_pos = float3(0, 0, 0);
		param[0].min_pos = float3(0, 0, 0);
	}
}
void PositionSort(int id) {
	if (param[0].max_pos.x < output[id].pos.x)param[0].max_pos.x = output[id].pos.x;
	if (param[0].max_pos.y < output[id].pos.y)param[0].max_pos.y = output[id].pos.y;
	if (param[0].max_pos.z < output[id].pos.z)param[0].max_pos.z = output[id].pos.z;
	if (param[0].min_pos.x > output[id].pos.x)param[0].min_pos.x = output[id].pos.x;
	if (param[0].min_pos.y > output[id].pos.y)param[0].min_pos.y = output[id].pos.y;
	if (param[0].min_pos.z > output[id].pos.x)param[0].min_pos.z = output[id].pos.z;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothSpring(uint3 th_id : SV_GroupID){
	int dim = sqrt(input[0].vertex_size);
	int id = (th_id.x * dim) + th_id.y;
	//int id = th_id.x;
	float dt = 0.034;
	if (id >= input[0].vertex_size)return;
	firstSetting(id);
	if (isFixed(input[id].col)) {
		output[id].pos = input[id].pos;
		return;
	}
	float3 f = add(ShareSolver(id), StructuralSolver(id));
	f = add(f, BendingSolver(id));
	output[id].spring.force = add(input[id].spring.force, f);

	float3 v = scale(output[id].spring.force, 1 / (input[id].spring.mass + input[id].spring.mass));
	v = scale(v, dt);
	output[id].spring.velocity = add(input[id].spring.velocity, v);
	v = scale(output[id].spring.velocity, dt);
	output[id].pos = add(input[id].pos, v);
	//óÕÇÉ[ÉçÇ…Ç∑ÇÈ
	output[id].spring.force = float3(0, 0, 0);
	PositionSort(id);
}