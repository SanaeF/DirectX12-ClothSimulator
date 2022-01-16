#include "RootSignature/RSNewPosition.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "ClothProp/SpringData.hlsli"
#include "ClothProp/NewPosData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
#include "SpringCalculator/CollisionCalculator.hlsli"

//CPUに送る構造体
RWStructuredBuffer<ModelVertex>out_vert : register(u0);
RWStructuredBuffer<SpringData>out_spring : register(u1);
RWStructuredBuffer<PositionFrame>out_frame : register(u2);
//CPUから送られてくる構造体
RWStructuredBuffer<SimulateParam>in_param : register(u3);
RWStructuredBuffer<ModelVertex>in_vert : register(u4);
RWStructuredBuffer<SpringData>in_spring : register(u5);
RWStructuredBuffer<PolygonModel> polygon_model : register(u6);

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
void ClothNewPosition(uint3 th_id : SV_GroupID) {
	SimulateParam param = in_param[0];
	int dim = sqrt(param.vert_max);
	int id = (th_id.x * dim) + th_id.y;
	if (id >= param.vert_max)return;
	if (isnan(in_spring[id].velocity.x))in_spring[id].velocity = float3(0, 0, 0);
	firstSetting(id);
	if (isFixed(in_vert[id].color))return;
	float dt = param.dt/100;
	float3 v = scale(out_spring[id].force, in_spring[id].mass / (in_spring[id].mass + in_spring[id].mass));
	v = scale(v, dt);
	out_spring[id].velocity = add(in_spring[id].velocity, v);
	v = scale(out_spring[id].velocity, dt);
	out_vert[id].pos = add(in_vert[id].pos, v);
	//力をゼロにする
	out_spring[id].force = float3(0, 0, 0);

	//頂点の法線
	int size = polygon_model[id].polygon_num;
	float3 normal = float3(0, 0, 0);
	for (int ite = 0; ite < size; ite++) {
		//有効な三角形か確認
		int3 polygon = polygon_model[id].id[ite];
		normal = add(normal, createNormal(in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos));
	}
	//normal = scale(normal, 1 / size);
	normal = normalize(normal);
	out_vert[id].normal = float4(normal.x, normal.y, normal.z, in_vert[id].normal.w);
	//if (param.is_no_add_vel)out_spring[id].velocity = in_spring[id].velocity;
	PositionSort(id);
}