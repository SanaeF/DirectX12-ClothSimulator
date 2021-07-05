#include "ClothData.hlsli"

//StructuredBuffer<Vertex> vert : register(u0);
//StructuredBuffer<Vertex> pre_vert : register(u1);
//
//RWStructuredBuffer<SpringData> spring_data : register(u2);
//
//int vertex_max : register(u3)
;
[numthreads(1, 1, 1)]
void ClothCollider( uint3 DTid : SV_DispatchThreadID ){
	//for (int ite = 0; ite < vertex_max; ite++) {
	//	//全頂点を総当たり
	//	for (int ite2 = 0; ite2 < vertex_max; ite2++) {
	//		//現在の頂点同士の距離
	//		float length = distance(vert[ite].position, vert[ite2].position);
	//		//デフォルトの頂点同士の距離
	//		float pre_length = distance(pre_vert[ite].position, pre_vert[ite2].position);
	//		pre_length = pre_length / 2;
	//		//あたり判定の範囲内を検出
	//		if (length < pre_length) {
	//			float3 n = sub(vert[ite].position, vert[ite2].position);
	//			n = normalize(n);
	//			float3 f = dot(n, pre_length / length);
	//			//力の代入
	//			spring_data[ite].force = add(spring_data[ite].force, f);
	//		}
	//	}
	//}
}