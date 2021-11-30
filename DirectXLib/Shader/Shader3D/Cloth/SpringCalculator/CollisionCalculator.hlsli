//球と直線の当たり判定
bool isHitSphereAndLine(float3 p1, float3 p2, float3 sph, float r) {
	float3 p1_sph = subtract(sph, p1);
	float3 p2_sph = subtract(sph, p2);
	float3 p1_p2 = subtract(p2, p1);
	if (dot(p1_sph, p1_p2) * dot(p2_sph, p1_p2) <= 0) {
		float theta = subtendedAngle(p1_sph, p1_p2);
		float d = abs(mul(p1_p2, p1_sph)) / distance(p2, p1);
		if (d <= r)return true;
	}
	return false;
}
//三角形の法線を求める
float3 createNormal(float3 p0,float3 p1,float3 p2) {
	float3 p0_p1 = subtract(p1, p0);
	float3 p0_p2 = subtract(p2, p0);
	float theta = subtendedAngle(p0_p1, p0_p2);
	return cross(p0_p1, p0_p2);
}
//三角形と直線の交点
float3 CrossPolygonAndLine(float3 p1, float3 p2, float3 plane0, float3 plane1, float3 plane2) {
	//頂点から法線を求める
	float3 vn = createNormal(plane0, plane1, plane2);
	//平面との距離を求める
	float3 v1 = subtract(p1, plane0);
	float d1 = abs(dot(vn, v1)) / abs(vn);
	float3 v2 = subtract(p2, plane0);
	float d2 = abs(dot(vn, v2)) / abs(vn);
	//内分比を求める。
	float ratio = d1 / (d1 + d2);
	//交点を求める
	float3 v3 = add(scale(v1, 1 - ratio), scale(v2, ratio));
	float3 p3 = add(plane0, v3);
	return p3;
}
//三角形と点の当たり判定
bool isHitPolygonAndPoint(float3 plane0, float3 plane1, float3 plane2, float3 p) {
	//頂点の外積を求める
	float3 p0_p1 = subtract(plane1, plane0);
	float3 p1_p = subtract(p, plane1);

	float3 p1_p2 = subtract(plane2, plane1);
	float3 p2_p = subtract(p, plane2);

	float3 p2_p0 = subtract(plane0, plane2);
	float3 p0_p = subtract(p, plane0);

	float3 A = cross(p0_p1, p1_p);
	float3 B = cross(p1_p2, p2_p);
	float3 C = cross(p2_p0, p0_p);
	float dot_AB = dot(A, B);
	float dot_AC = dot(A, C);
	if (dot_AB > 0 && dot_AC > 0)return true;
	return false;
}
//三角形と直線の当たり判定
bool isHitPolygonAndLine(float3 p1, float3 p2, float3 plane0, float3 plane1, float3 plane2) {
	//交点を求める
	float3 p3 = CrossPolygonAndLine(p1, p2, plane0, plane1, plane2);
	//ポリゴン内に点があるかを求める
	return isHitPolygonAndPoint(plane0, plane1, plane2, p3);
}
//平面と直線の当たり判定
bool isHitPlneAndLine(float3 p1, float3 p2, float3 plane0, float3 plane1, float3 plane2, float3 plane3) {
	//一番遠い頂点(plane0の対角)をplane[2]に割り当てる。それ以外で残りの頂点(plane0以外)を割り当てる
	float3 plane[3] = { plane1, plane2, plane3 };
	float dist_max = 0;
	for (int ite = 0; ite < 3; ite++) {
		float dist = distance(plane0, plane[ite]);
		if (dist > dist_max)dist_max = dist;
	}
	for (int ite = 0; ite < 3; ite++) {
		if (distance(plane0, plane[ite]) == dist_max) {
			float3 max_plane = plane[ite];
			plane[ite] = plane[2];
			plane[2] = max_plane;
			break;
		}
	}
	//平面をポリゴンで分けて判定
	bool is_hit=false;
	is_hit = isHitPolygonAndLine(p1, p2, plane0, plane[0], plane[2]);
	if(!is_hit)is_hit = isHitPolygonAndLine(p1, p2, plane0, plane[1], plane[2]);
	return is_hit;
}