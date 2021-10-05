//#include "VectLib.hlsli"

float mulAdd(float3 vec1, float3 vec2) {
	float x = vec1.x * vec2.x;
	float y = vec1.y * vec2.y;
	float z = vec1.z * vec2.z;
	float result = x + y + z;
	return sqrt(result);
}
float3 normalize(float3 pos) {
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	float length = mulAdd(pos, pos);
	float3 result = float3(0, 0, 0);
	if (length != 0) {
		result.x = pos.x / length;
		result.y = pos.y / length;
		result.z = pos.z / length;
	}
	return result;
}
float3 scale(float3 vec, float size) {
	float3 result;
	result.x = vec.x * size;
	result.y = vec.y * size;
	result.z = vec.z * size;
	return result;
}
float3 subtract(float3 vec1, float3 vec2) {
	float3 result;
	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;
	return result;
}
float distance(float3 vec1, float3 vec2) {
	float3 dist = subtract(vec1, vec2);
	float result = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.z * dist.z));
	return result;
}

float3 CalcForce(float3 pos1, float3 pos2, float length, float constant) {
	float3 result;
	float3 n = subtract(pos1, pos2);
	float leg = mulAdd(n, n);
	n = normalize(n);//���K��2
	float f = (length - leg) * constant;
	result = scale(n, f);
	return result;
}

bool isFixed(float3 color) {
	if (color.x == 1.f &&
		color.y == 0.f &&
		color.z == 0.f)return true;
	return false;
}