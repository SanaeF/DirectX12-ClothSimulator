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
RWStructuredBuffer<PolygonModel> polygon_model : register(u8);

//対象の頂点から、過去の頂点に向かって戻す
float3 extrudeSphereForce(float3 vec, float dist, float power) {
	vec = normalize(vec);
	float size = dist + power;
	//if (size > 1)size = 1;
	return scale(vec, size);
}

//交点の位置に向かって交点までの距離とサイズ分を押し出す
float3 extrudeForce(float3 now_p1, float3 last_p, float3 cross_p, float power) {
	float dist = distance(now_p1, cross_p);
	float3 vec = subtract(last_p, now_p1);
	vec = normalize(vec);
	float size = dist + power;
	//if (size > 1)size = 1;
	return scale(vec, size);
}
//対象の頂点と同一の頂点を持ったポリゴンがないかを確認
bool isEffectivPolygon(int id, int3 polygon_id) {
	bool result = true;
	if (polygon_id.x == id &&
		polygon_id.y == id &&
		polygon_id.z == id)result = false;
	return result;
}
//二つの頂点に対して何かしらの当たり判定を取る
bool isHit(int id1, int id2) {
	bool result = false;
	int size = polygon_model[id2].polygon_num;
	//三角形の数だけループ
	for (int ite = 0; ite < size; ite++) {
		//有効な三角形か確認
		int3 polygon = polygon_model[id2].id[ite];
		if (isEffectivPolygon(id1, polygon)) {
			//三角形と点の当たり判定を取る
			if (isHitPolygonAndPoint(
				in_vert[polygon.x].pos,
				in_vert[polygon.y].pos,
				in_vert[polygon.z].pos,
				in_vert[id1].pos)
				)result = true;
		}
	}
	return result;
}

[RootSignature(RS)]
[numthreads(1, 1, 1)] 
void ClothCollider(uint3 th_id : SV_GroupID) {
	CollisionParam param = collision_param[0];
	int dim = sqrt(param.vertex_size);
	int id1 = (th_id.x * dim) + th_id.y;
	out_spring[id1] = spring[id1];
	if (isFixed(in_vert[id1].color))return;
	int space_id = split_area[id1];

	//空間内同士の当たり判定
	int hit_type = 1;
	if (in_vert[id1].pos.x == last_vert[id1].pos.x &&
		in_vert[id1].pos.y == last_vert[id1].pos.y && 
		in_vert[id1].pos.z == last_vert[id1].pos.z) return;
	int target_size = space[space_id].count;
	float hit_size = param.hit_size;

	float3 normal_vec = float3(in_vert[id1].normal.x, in_vert[id1].normal.y, in_vert[id1].normal.z);
	out_spring[id1].force = add(out_spring[id1].force, scale(normal_vec, hit_size/2));

	for (int ite1 = 0; ite1 < target_size; ite1++) {
		int id2 = space[ite1].id[space_id];
		if (id1 == id2)continue;
		//二つの頂点に対して何かしらの当たり判定を取る
		bool is_hit_sphere = false;
		//貫通していない場合の当たり判定
		float hit_dist = distance(in_vert[id1].pos, in_vert[id2].pos);
		if (hit_dist < hit_size * 10) {
			float3 vec = subtract(in_vert[id2].pos, in_vert[id1].pos);
			float3 f = extrudeSphereForce(vec, hit_dist, hit_size * 10);
			out_spring[id2].force = add(out_spring[id2].force, f);

			vec = subtract(last_vert[id1].pos, in_vert[id1].pos);
			f = extrudeSphereForce(vec, hit_dist, hit_size * 1);
			out_spring[id1].force = add(out_spring[id1].force, f);
			//is_hit_sphere = true;
		}
		//自身が持つ三角形と対象の頂点(P0,P1)の判定
		int pol_size = polygon_model[id1].polygon_num;
		for (int ite2 = 0; ite2 < pol_size; ite2++) {
			//有効な三角形か確認
			int3 polygon = polygon_model[id1].id[ite2];
			if (isEffectivPolygon(id2, polygon)) {
				//三角形と直線の交点を求める
				float3 cross_p =
					CrossPolygonAndLine(
						in_vert[id2].pos, last_vert[id2].pos, in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos
					);
				//三角形と直線の交点で当たり判定を取る
				if (isHitPolygonAndPoint(in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos, cross_p)) {
					float3 f = extrudeForce(in_vert[id2].pos, last_vert[id2].pos, cross_p, hit_size * 10);
					if (isnan(f.x) && isnan(f.y) && isnan(f.z))f = float3(0, 0, 0);
					out_spring[id2].force = add(out_spring[id2].force, f);
				}
			}
		}
		//三角形の数だけループ
		pol_size = polygon_model[id2].polygon_num;
		for (int ite2 = 0; ite2 < pol_size; ite2++) {
			//有効な三角形か確認
			int3 polygon = polygon_model[id2].id[ite2];
			if (isEffectivPolygon(id1, polygon)) {
				//三角形と直線の交点を求める
				float3 cross_p =
					CrossPolygonAndLine(
						in_vert[id1].pos, last_vert[id1].pos, in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos
					);
				//三角形と直線の交点で当たり判定を取る
				if (isHitPolygonAndPoint(in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos, cross_p)) {
					float3 f = extrudeForce(in_vert[id1].pos, last_vert[id1].pos, cross_p, hit_size * 10);
					if (isnan(f.x) && isnan(f.y) && isnan(f.z))f = float3(0, 0, 0);
					out_spring[id1].force = add(out_spring[id1].force, f);
				}
			}
		}
	}
}