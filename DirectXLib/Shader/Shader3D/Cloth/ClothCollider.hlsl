#include "ClothProp/RSClothCollider.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "ClothProp/CollisionData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
#include "SpringCalculator/CollisionCalculator.hlsli"

RWStructuredBuffer<ModelVertex>out_vert : register(u0);
RWStructuredBuffer<SpringData>out_spring : register(u1);
RWStructuredBuffer<CollisionParam>collision_param : register(u2);
RWStructuredBuffer<ModelVertex>pre_vert : register(u3);
RWStructuredBuffer<ModelVertex>in_vert : register(u4);
RWStructuredBuffer<SpaceData>space : register(u5);
RWStructuredBuffer<int>split_area : register(u6);
RWStructuredBuffer<ModelVertex>last_vert : register(u7);
RWStructuredBuffer<SpringData>spring : register(u8);
RWStructuredBuffer<MassModel> mass_model : register(u9);

float3 solver(float3 now_p1, float3 last_p1, float3 p2, float hit_size) {
	int test_type = 1;
	float3 result = float3(0, 0, 0);
	if (test_type == 0) {
		//last_p1モデルが当たり判定p2に食い込んでいるかどうか
		if (distance(last_p1, p2) < hit_size) {
			//食い込んでいた
			float3 n = subtract(now_p1, p2);
			n = normalize(n);
			result = scale(n, hit_size);
		}
		else {
			float3 n = subtract(last_p1, now_p1);
			float3 nowP1_P2 = subtract(p2, now_p1);
			float3 nowP1_lastP1 = subtract(last_p1, now_p1);
			float theta = subtendedAngle(nowP1_P2, nowP1_lastP1);
			float dist = abs(mulAdd(nowP1_P2, nowP1_P2)) * cos(theta);
			n = normalize(n);
			result = scale(n, dist + hit_size);
		}
	}
	if (test_type == 1) {
		float3 n = subtract(last_p1, now_p1);
		float3 nowP1_P2 = subtract(p2, now_p1);
		float3 nowP1_lastP1 = subtract(last_p1, now_p1);
		float theta = subtendedAngle(nowP1_P2, nowP1_lastP1);
		float dist = abs(mulAdd(nowP1_P2, nowP1_P2)) * cos(theta);
		dist = distance(last_p1, now_p1);
		n = normalize(n);
		result = scale(n, dist + hit_size);
	}
	return result;
}
float3 solverPlane(float3 now_p1, float3 last_p1, int4 plane_id) {
	float3 result = float3(0, 0, 0);
	float3 p3 = CrossPolygonAndLine(
		now_p1, last_p1,
		in_vert[plane_id.x].pos,
		in_vert[plane_id.y].pos,
		in_vert[plane_id.z].pos
		);
	float dist = distance(p3, now_p1);
	float3 n = subtract(p3, now_p1);
	n = normalize(n);
	result = scale(n, dist);
	return result;
}
int4 createPlanePoint(int id1, int id2) {
	int plane_id[4] = {
		mass_model[id2].id4,
		mass_model[id2].id5,
		mass_model[id2].id6,
		mass_model[id2].id7
	};
	if (mass_model[id2].id3 == -1) {
		plane_id[0] = mass_model[id2].id0;
		plane_id[1] = mass_model[id2].id1;
		plane_id[2] = mass_model[id2].id4;
		plane_id[3] = mass_model[id2].id5;
	}
	if (mass_model[id2].id2 == -1) {
		plane_id[0] = mass_model[id2].id0;
		plane_id[1] = mass_model[id2].id1;
		plane_id[2] = mass_model[id2].id4;
		plane_id[3] = id2;
	}
	for (int ite = 0; ite < 4; ite++) {
		if (plane_id[ite] == id1)plane_id[ite] = id2;
	}
	return int4(plane_id[0], plane_id[1], plane_id[2], plane_id[3]);
}

bool isHit(int id1,int id2) {
	int4 id = createPlanePoint(id1, id2);
	if (mass_model[id2].id1 == -1)return false;
	return isHitPlneAndLine(
		in_vert[id1].pos, last_vert[id1].pos,
		in_vert[id.x].pos,
		in_vert[id.y].pos,
		in_vert[id.z].pos,
		in_vert[id.w].pos
	);
}

void firstSetting(int id) {
	out_vert[id] = in_vert[id];
	out_spring[id]= spring[id];
}
void newPosition(int id1, int id2) {
	float dt = 0.0036;
	float3 v = scale(out_spring[id1].force, spring[id1].mass / (spring[id1].mass + spring[id2].mass));
	v = scale(v, dt);
	out_spring[id1].velocity = add(spring[id1].velocity, v);
	v = scale(out_spring[id1].velocity, dt);
	out_vert[id1].pos = add(in_vert[id1].pos, v);
	//力をゼロにする
	//out_spring[id1].force = float3(0, 0, 0);
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

	//空間内同士の当たり判定
	hit_dist = 0.0138*20;
	for (int ite = 0; ite < space[space_id].count; ite++) {
		int id2 = space[ite].id[space_id];
		if (id1 == id2)continue;
		if (isHit(id1, id2)) {
			//float3 f = solverPlane(in_vert[id1].pos, last_vert[id1].pos, createPlanePoint(id1, id2));
			float3 f = solver(in_vert[id1].pos, last_vert[id1].pos, in_vert[id2].pos, hit_dist);
			out_spring[id1].force = add(spring[id1].force, f);
			newPosition(id1, id2);
			//f = scale(f, 2);
			out_spring[id2].force = subtract(spring[id2].force, f);
			newPosition(id2, id1);
		}
	}
	////実験用球体
	//float3 ball = float3(15, -2, 0);
	//hit_dist = 1.3;
	//if (isHitSphereAndLine(in_vert[id1].pos, last_vert[id1].pos, ball, hit_dist)) {
	//	float f = solver(in_vert[id1].pos, last_vert[id1].pos, ball, hit_dist);
	//	spring[id1].force = add(spring[id1].force, f);
	//}
}