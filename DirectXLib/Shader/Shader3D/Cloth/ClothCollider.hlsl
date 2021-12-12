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

//交点の位置に向かって交点までの距離とサイズ分を押し出す
float3 extrudeForce(float3 now_p1, float3 cross_p, float power) {
	float dist = distance(now_p1, cross_p);
	float3 vec = subtract(cross_p, now_p1);
	vec = normalize(vec);
	return scale(vec, dist + power);
}
//対象の頂点と同一の頂点を持ったポリゴンがないかを確認
bool isEffectivPolygon(int id, int3 polygon_id) {
	bool result = true;
	if (polygon_id.x == id ||
		polygon_id.y == id ||
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
	if (in_vert[id1].pos.x== last_vert[id1].pos.x  ||
		in_vert[id1].pos.y == last_vert[id1].pos.y || 
		in_vert[id1].pos.z == last_vert[id1].pos.z) return;
	int size = space[space_id].count;
	//size = param.vertex_size;
	for (int ite = 0; ite < size; ite++) {
		int id2 = space[ite].id[space_id];
		//id2 = ite;
		if (id1 == id2)continue;
		//二つの頂点に対して何かしらの当たり判定を取る
		int size = polygon_model[id2].polygon_num;
		//三角形の数だけループ
		for (int ite = 0; ite < size; ite++) {
			//有効な三角形か確認
			int3 polygon = polygon_model[id2].id[ite];
			if (isEffectivPolygon(id1, polygon)) {
				//三角形と直線の交点を求める
				float3 cross_p =
					CrossPolygonAndLine(
						in_vert[id1].pos, last_vert[id1].pos, in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos
					);
				//三角形と直線の交点で当たり判定を取る
				if (isHitPolygonAndPoint(in_vert[polygon.x].pos, in_vert[polygon.y].pos, in_vert[polygon.z].pos, cross_p)) {
					float3 f = extrudeForce(in_vert[id1].pos, cross_p, param.power/100);
					if (isnan(f.x)&& isnan(f.y)&& isnan(f.z))f = float3(0, 0, 0);
					out_spring[id1].force = add(out_spring[id1].force, f);
				}
			}
		}
	}
}