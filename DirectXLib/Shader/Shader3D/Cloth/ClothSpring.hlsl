#include "RootSignature/RSClothSpring.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/SpringData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
//CPUに送る構造体
RWStructuredBuffer<SpringData>out_spring : register(u0);
//CPUから送られてくる構造体
RWStructuredBuffer<SimulateParam> sim_param : register(u1);
RWStructuredBuffer<MassModel> mass_model : register(u2);
RWStructuredBuffer<ModelVertex> pre_vert : register(u3);
RWStructuredBuffer<ModelVertex> vertex :register(u4);
RWStructuredBuffer<SpringData> spring : register(u5);//受け取れていない

float3 StructuralSolver(int id) {
	SimulateParam param = sim_param[0];
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id0;
		if (ite == 1)id2 = mass_model[id].id1;
		if (ite == 2)id2 = mass_model[id].id2;
		if (ite == 3)id2 = mass_model[id].id3;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(
			vertex[id].pos,
			vertex[id2].pos,
			Natulength,
			param.tension.stretch,
			param.tension.shrink,
			param.k
		);
		result = add(result, f);
	}
	return result;
}
float3 CompressionSolver(int id) {//質点の向きに働く力
	SimulateParam param = sim_param[0];
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id0;
		if (ite == 1)id2 = mass_model[id].id1;
		if (ite == 2)id2 = mass_model[id].id2;
		if (ite == 3)id2 = mass_model[id].id3;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(
			vertex[id].pos,
			vertex[id2].pos,
			Natulength,
			param.compress.stretch,
			param.compress.shrink,
			param.k
		);
		result = add(result, f);
	}
	return result;
}
float3 ShareSolver(int id) {
	SimulateParam param = sim_param[0];
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id4;
		if (ite == 1)id2 = mass_model[id].id5;
		if (ite == 2)id2 = mass_model[id].id6;
		if (ite == 3)id2 = mass_model[id].id7;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(
			vertex[id].pos,
			vertex[id2].pos,
			Natulength,
			param.share.stretch,
			param.share.shrink,
			param.k
		);
		result = add(result, f);
	}
	return result;
}
float3 BendingSolver(int id) {
	SimulateParam param = sim_param[0];
	float3 result = float3(0.f, 0.f, 0.f);
	int id2;
	for (int ite = 0; ite < 4; ite++) {
		if (ite == 0)id2 = mass_model[id].id8;
		if (ite == 1)id2 = mass_model[id].id9;
		if (ite == 2)id2 = mass_model[id].id10;
		if (ite == 3)id2 = mass_model[id].id11;
		if (id2 == -1)continue;
		float Natulength = distance(pre_vert[id].pos, pre_vert[id2].pos);//ばねの自然長
		float3 f = CalcForce(
			vertex[id].pos,
			vertex[id2].pos,
			Natulength,
			param.bend.stretch,
			param.bend.shrink,
			param.k
		);
		result = add(result, f);
	}
	return result;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothSpring(uint3 th_id : SV_GroupID){
	SimulateParam param = sim_param[0];
	int dim = sqrt(param.vert_max);
	int id = (th_id.x * dim) + th_id.y;
	float dt = param.dt;
	if (id >= param.vert_max)return;
	out_spring[id] = spring[id];
	if (isFixed(vertex[id].color))return;
	if (isNullData(spring[id].velocity))return;

	float3 f = float3(0, 0, 0);
	f = add(f, StructuralSolver(id));//構造
	f = add(f, CompressionSolver(id));//圧縮
	f = add(f, ShareSolver(id));//曲げ
	f = add(f, BendingSolver(id));//せん断
	out_spring[id].force = f;
}