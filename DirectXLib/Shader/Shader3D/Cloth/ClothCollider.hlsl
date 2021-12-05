#include "RootSignature/RSClothCollider.hlsli"
#include "ClothProp/ModelData.hlsli"
#include "ClothProp/ClothData.hlsli"
#include "ClothProp/SpringData.hlsli"
#include "ClothProp/CollisionData.hlsli"
#include "SpringCalculator/SpringCalculator.hlsli"
#include "SpringCalculator/CollisionCalculator.hlsli"

RWStructuredBuffer<SpringData>out_spring : register(u0);
RWStructuredBuffer<CollisionParam>collision_param : register(u1);
RWStructuredBuffer<ModelVertex>pre_vert : register(u2);
RWStructuredBuffer<ModelVertex>in_vert : register(u3);
RWStructuredBuffer<SpaceData>space : register(u4);
RWStructuredBuffer<int>split_area : register(u5);
RWStructuredBuffer<ModelVertex>last_vert : register(u6);
RWStructuredBuffer<SpringData>spring : register(u7);
RWStructuredBuffer<MassModel> mass_model : register(u8);

//交点の位置に向かって交点までの距離とサイズ分を押し出す
float3 extrudeForce(float3 now_p1, float3 cross_p, float hit_size) {
	float dist = distance(now_p1, cross_p);
	float3 vec = subtract(cross_p, now_p1);
	vec = normalize(vec);
	return scale(vec, dist + hit_size);
}
//平面の頂点を押し出す
float3 extrudePlane(float3 now_p1, float3 now_p2, float hit_size) {
	float dist = distance(now_p1, now_p2);
	float3 vec = subtract(now_p1, now_p2);
	vec = normalize(vec);
	return scale(vec, dist);
}
//直線と4質点平面から交点を求める
float3 crossPlane(float3 now_p1, float3 last_p1, int4 plane_id) {
	float3 result = float3(0, 0, 0);
	float3 p3 = CrossPolygonAndLine(
		now_p1, last_p1,
		in_vert[plane_id.x].pos,
		in_vert[plane_id.y].pos,
		in_vert[plane_id.z].pos
		);
	return p3;
}
int4 createPlanePoint(int id1, int id2) {
	int plane_id[4] = {
		mass_model[id2].id0,
		mass_model[id2].id1,
		mass_model[id2].id2,
		mass_model[id2].id3
	};
	if (mass_model[id2].id3 == -1)plane_id[3] = id2;
	if (mass_model[id2].id2 == -1) {
		plane_id[2] = mass_model[id2].id4;
		plane_id[3] = mass_model[id2].id5;
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

[RootSignature(RS)]
[numthreads(1, 1, 1)] 
void ClothCollider(uint3 th_id : SV_GroupID) {
	CollisionParam param = collision_param[0];
	int dim = sqrt(param.vertex_size);
	int id1 = (th_id.x * dim) + th_id.y;
	if (id1 == 0) {
		for(int ite=0;ite< param.vertex_size;ite++)out_spring[ite]= spring[ite];
	}
	if (isFixed(in_vert[id1].color))return;
	int space_id = split_area[id1];
	float hit_dist = 0;

	//空間内同士の当たり判定
	hit_dist = 0.0138*100;
	int hit_type = 0;
	if (in_vert[id1].pos.x== last_vert[id1].pos.x  ||
		in_vert[id1].pos.y == last_vert[id1].pos.y || 
		in_vert[id1].pos.z == last_vert[id1].pos.z) return;
	for (int ite = 0; ite < space[space_id].count; ite++) {
		int id2 = space[ite].id[space_id];
		if (id1 == id2)continue;
		if (isHit(id1, id2)) {
			int4 plane_id = createPlanePoint(id1, id2);
			float3 cross_p = crossPlane(in_vert[id1].pos, last_vert[id1].pos, plane_id);
			if (hit_type == 0) {//頂点を押し戻して、当たった平面にも反発力を与えない
				float3 f = extrudeForce(in_vert[id1].pos, cross_p, hit_dist);
				out_spring[id1].force = add(out_spring[id1].force, f);
			}
			if (hit_type == 1) {//当たった面だけを押し出すタイプ
				float3 f = extrudePlane(in_vert[id1].pos, in_vert[id2].pos, hit_dist);
				out_spring[id2].force = add(out_spring[id2].force, f);
			}
		}
	}
	//if(hit_count>0)newPosition(id1);
	////実験用球体
	//float3 ball = float3(15, -2, 0);
	//hit_dist = 1.3;
	//if (isHitSphereAndLine(in_vert[id1].pos, last_vert[id1].pos, ball, hit_dist)) {
	//	float f = solver(in_vert[id1].pos, last_vert[id1].pos, ball, hit_dist);
	//	spring[id1].force = add(spring[id1].force, f);
	//}
}