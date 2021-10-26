#include "ClothProp/CSRootSig.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
//CPUに送る構造体
RWStructuredBuffer<ModelVertex> out_vert : register(u0);
RWStructuredBuffer<SpringData>out_spring : register(u1);
RWStructuredBuffer<MaxPosition>out_max_pos : register(u2);
//CPUから送られてくる構造体
RWStructuredBuffer<SimulateParam> sim_param : register(u3);
RWStructuredBuffer<MassModel> mass_model : register(u4);
RWStructuredBuffer<ModelVertex> pre_vert : register(u5);
RWStructuredBuffer<ModelVertex> vertex :register(u6);
RWStructuredBuffer<SpringData> spring : register(u7);//受け取れていない

float3 StructuralSolver(int id) {
	int tension = 15;
	int tension_damping = 15;
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 2; ite++) {
		if (ite == 0)id2 = mass_model[id].id2;
		if (ite == 1)id2 = mass_model[id].id3;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(vertex[id].pos, vertex[id2].pos, Natulength, tension, tension_damping);
		result = add(result, f);
	}
	return result;
}
float3 CompressionSolver(int id) {
	int tension = 15;
	int tension_damping = 5;
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id0;
		if (ite == 1)id2 = mass_model[id].id1;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(vertex[id].pos, pre_vert[id].pos, Natulength, tension, tension_damping);
		result = add(result, f);
	}
	return result;
}
float3 ShareSolver(int id) {
	int shear = 15;
	int shear_damping = 5;
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id4;
		if (ite == 1)id2 = mass_model[id].id5;
		if (ite == 2)id2 = mass_model[id].id6;
		if (ite == 3)id2 = mass_model[id].id7;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(vertex[id].pos, vertex[id2].pos, Natulength, shear, shear_damping);
		result = add(result, f);
	}
	return result;
}
float3 BendingSolver(int id) {
	int tension = 25;
	int tension_damping = 2;
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id8;
		if (ite == 1)id2 = mass_model[id].id9;
		if (ite == 2)id2 = mass_model[id].id10;
		if (ite == 3)id2 = mass_model[id].id11;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(vertex[id].pos, vertex[id2].pos, Natulength, tension, tension_damping);
		result = add(result, f);
	}
	return result;
}

void firstSetting(int id) {
	out_vert[id] = vertex[id];
	out_spring[id] = spring[id];
	if (id == 0) {
		out_max_pos[0].max_pos = float3(0, 0, 0);
		out_max_pos[0].min_pos = float3(0, 0, 0);
	}
}
void PositionSort(int id) {
	if (out_max_pos[0].max_pos.x < out_vert[id].pos.x)out_max_pos[0].max_pos.x = out_vert[id].pos.x;
	if (out_max_pos[0].max_pos.y < out_vert[id].pos.y)out_max_pos[0].max_pos.y = out_vert[id].pos.y;
	if (out_max_pos[0].max_pos.z < out_vert[id].pos.z)out_max_pos[0].max_pos.z = out_vert[id].pos.z;
	if (out_max_pos[0].min_pos.x > out_vert[id].pos.x)out_max_pos[0].min_pos.x = out_vert[id].pos.x;
	if (out_max_pos[0].min_pos.y > out_vert[id].pos.y)out_max_pos[0].min_pos.y = out_vert[id].pos.y;
	if (out_max_pos[0].min_pos.z > out_vert[id].pos.x)out_max_pos[0].min_pos.z = out_vert[id].pos.z;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothSpring(uint3 th_id : SV_GroupID){
	SimulateParam param = sim_param[0];
	out_max_pos[0].is_simulated = true;
	int dim = sqrt(param.vert_max);
	int id = (th_id.x * dim) + th_id.y;
	float dt = 0.026;
	if (id >= param.vert_max)return;
	firstSetting(id);
	if (isFixed(vertex[id].color))return;
	if (isNullData(spring[id].velocity))return;
	float3 f = float3(0, 0, 0);
	f = add(f, StructuralSolver(id));
	f = add(f, CompressionSolver(id));
	f = add(f, ShareSolver(id));
	f = add(f, BendingSolver(id));
	out_spring[id].force = f;

	float3 v = scale(out_spring[id].force, 1 / (spring[id].mass + spring[id].mass));
	v = scale(v, dt);
	out_spring[id].velocity = add(spring[id].velocity, v);
	v = scale(out_spring[id].velocity, dt);
	out_vert[id].pos = add(vertex[id].pos, v);

	////力をゼロにする
	//out_spring[id].force = spring[id].velocity;
	//PositionSort(id);
}

//
//void SpringForce(int step, int id1, int id2, float k, float shrink, float stretch) {
//	float width = 1;
//	float rest = distance(input[id2].pre_pos, input[id1].pre_pos);
//	float d = distance(input[id2].pos, input[id1].pos);
//	float f = (d - rest) * k;
//	if (f >= 0)f = f * shrink;//伸びる長さであれば減退
//	else f = f * stretch;
//
//	float3 dx = subtract(input[id2].pos, input[id1].pos);
//	dx = normalize(dx);
//	dx = scale(dx, f);
//	dx = scale(dx, step * step * 0.5 / input[id1].spring.mass);
//	float3 p1_dx = scale(dx, width / (width + width));
//	output[id1].pos = add(input[id1].pos, p1_dx);
//	float3 p2_dx = scale(dx, width / (width + width));
//	output[id2].pos = subtract(input[id2].pos, p2_dx);
//}
//
//[RootSignature(RS)]
//[numthreads(1, 1, 1)]
//void ClothSpring(uint3 th_id : SV_GroupID) {
//	int dim = sqrt(input[0].vertex_size);
//	int id = (th_id.x * dim) + th_id.y;
//	int id2;
//	int step = input[0].step;
//	//int id = th_id.x;
//	float dt = 0.026;
//	if (id >= input[0].vertex_size)return;
//	firstSetting(id);
//	if (isFixed(input[id].col)) {
//		output[id].pos = input[id].pos;
//		return;
//	}
//	//structural
//	for (int ite = 0; ite < 4; ite++) {
//		if (ite == 0)id2 = input[id].id0;
//		if (ite == 1)id2 = input[id].id1;
//		if (ite == 2)id2 = input[id].id2;
//		if (ite == 3)id2 = input[id].id3;
//		SpringForce(step, id, id2, 10, 15, 15);
//	}
//	//sharing
//	for (int ite = 0; ite < 4; ite++) {
//		if (ite == 0)id2 = input[id].id4;
//		if (ite == 1)id2 = input[id].id5;
//		if (ite == 2)id2 = input[id].id6;
//		if (ite == 3)id2 = input[id].id7;
//		SpringForce(step, id, id2, 10, 15, 5);
//	}
//	//bending
//	for (int ite = 0; ite < 4; ite++) {
//		if (ite == 0)id2 = input[id].id8;
//		if (ite == 1)id2 = input[id].id9;
//		if (ite == 2)id2 = input[id].id10;
//		if (ite == 3)id2 = input[id].id11;
//		SpringForce(step, id, id2, 10, 25, 2);
//	}
//	PositionSort(id);
//}