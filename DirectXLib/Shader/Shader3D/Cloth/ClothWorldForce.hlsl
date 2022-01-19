#include "RootSignature/RSClothWorldForce.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "ClothProp/SpringData.hlsli"
#include "ClothProp/NewPosData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"

//CPUに送る構造体
RWStructuredBuffer<ModelVertex>out_vert : register(u0);
RWStructuredBuffer<SpringData>out_spring : register(u1);
RWStructuredBuffer<PositionFrame>out_frame : register(u2);
//CPUから送られてくる構造体
RWStructuredBuffer<SimulateParam>in_param : register(u3);
RWStructuredBuffer<ModelVertex>in_vert : register(u4);
RWStructuredBuffer<SpringData>in_spring : register(u5);

void PositionSort(int id) {
	if (out_frame[0].max_pos.x < out_vert[id].pos.x)out_frame[0].max_pos.x = out_vert[id].pos.x;
	if (out_frame[0].max_pos.y < out_vert[id].pos.y)out_frame[0].max_pos.y = out_vert[id].pos.y;
	if (out_frame[0].max_pos.z < out_vert[id].pos.z)out_frame[0].max_pos.z = out_vert[id].pos.z;
	if (out_frame[0].min_pos.x > out_vert[id].pos.x)out_frame[0].min_pos.x = out_vert[id].pos.x;
	if (out_frame[0].min_pos.y > out_vert[id].pos.y)out_frame[0].min_pos.y = out_vert[id].pos.y;
	if (out_frame[0].min_pos.z > out_vert[id].pos.x)out_frame[0].min_pos.z = out_vert[id].pos.z;
}

void firstSetting(int id) {
	out_vert[id] = in_vert[id];
	out_spring[id] = in_spring[id];
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothWorldForce(uint3 th_id : SV_GroupID) {
	SimulateParam param = in_param[0];
	int dim = sqrt(param.vert_max);
	int id = (th_id.x * dim) + th_id.y;
	if (id >= param.vert_max)return;
	if (isnan(in_spring[id].velocity.x))in_spring[id].velocity = float3(0, 0, 0);
	firstSetting(id);
	if (isFixed(in_vert[id].color))return;
	
	param.dt = param.dt / 1000;
	out_spring[id].mass = param.grid_mass;
	//重力を加える
	out_spring[id].force.y -= out_spring[id].mass * param.gravity;
	//風力を加える
	float r1 = param.time / 30;
	out_spring[id].force = add(out_spring[id].force, scale(param.wind, sin(r1) * sin(r1) * 0.25f + 0.25f));
	//ダンピング
	float3 d = scale(out_spring[id].velocity, param.damping);
	out_spring[id].force = subtract(out_spring[id].force, d);
	float3 v = scale(out_spring[id].force, 1 / (out_spring[id].mass + out_spring[id].mass));
	v = scale(v, param.dt);
	out_spring[id].velocity = add(out_spring[id].velocity, v);
	v = scale(out_spring[id].velocity, param.dt);
	out_vert[id].pos = add(out_vert[id].pos, v);
	//力をゼロにする
	out_spring[id].force = float3(0, 0, 0);
	PositionSort(id);
}