#include "ClothProp/RSClothCollider.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "ClothProp/CollisionData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
RWStructuredBuffer<ModelVertex>out_vert : register(u0);
RWStructuredBuffer<CollisionParam>collision_param : register(u1);
RWStructuredBuffer<ModelVertex>pre_vert : register(u2);
RWStructuredBuffer<ModelVertex>in_vert : register(u3);
RWStructuredBuffer<MassModel> mass_model : register(u4);
RWStructuredBuffer<SpaceData>space : register(u5);
RWStructuredBuffer<int>split_area : register(u6);

float3 solver(float3 p1, float3 p2, float hit_dist, float now_dist) {
	float3 n = subtract(p1, p2);
	n = scale(n, hit_dist / now_dist);
	return add(p2, n);
}

void firstSetting(int id) {
	out_vert[id] = in_vert[id];
}
[RootSignature(RS)]
[numthreads(1, 1, 1)] 
void ClothCollider(uint3 th_id : SV_GroupID) {
	CollisionParam param = collision_param[0];
	int dim = sqrt(param.vertex_size);
	int id1 = (th_id.x * dim) + th_id.y;
	firstSetting(id1);
	if (isFixed(in_vert[id1].color))return;
	int space_id = split_area[id1];
	float hit_dist = 0;
	float now_dist = 0;
	float3 ball = float3(1, -2, 0);

	//実験用球体
	//now_dist = distance(in_vert[id1].pos, ball);
	//hit_dist = 2.3;
	//if (now_dist < hit_dist) {
	//	float3 last_pos = in_vert[id1].pos;
	//	out_vert[id1].pos = solver(in_vert[id1].pos, ball, hit_dist, now_dist);
	//}

	//空間内同士の当たり判定
	hit_dist = 0.0138;
	for (int ite = 0; ite < space[space_id].count; ite++) {
		int id2 = space[ite].id[space_id];
		if (id1 == id2)continue;
		now_dist = distance(in_vert[id1].pos, in_vert[id2].pos);
		if (now_dist < hit_dist) {
			float3 last_pos = in_vert[id1].pos;
			out_vert[id1].pos = solver(in_vert[id1].pos, in_vert[id2].pos, hit_dist, now_dist);
		}
	}

	//質点との当たり判定(不要?)
	for (int ite = 0; ite < 4; ite++) {
		int id2;
		if (ite == 0)id2 = mass_model[id1].id0;
		if (ite == 1)id2 = mass_model[id1].id1;
		if (ite == 2)id2 = mass_model[id1].id2;
		if (ite == 3)id2 = mass_model[id1].id3;
		if (id2 == -1)continue;
		now_dist = distance(in_vert[id1].pos, in_vert[id2].pos);
		if (now_dist < hit_dist) {
			float3 last_pos = in_vert[id1].pos;
			out_vert[id1].pos = solver(in_vert[id1].pos, in_vert[id2].pos, hit_dist, now_dist);
		}
	}
}