
//float mulAdd(float3 vec1, float3 vec2) {
//	float x = vec1.x * vec2.x;
//	float y = vec1.y * vec2.y;
//	float z = vec1.z * vec2.z;
//	float result = x + y + z;
//	return sqrt(result);
//}
//float3 normalize(float3 pos) {
//	float x = pos.x;
//	float y = pos.y;
//	float z = pos.z;
//	float length = mulAdd(pos, pos);
//	float3 result = float3(0, 0, 0);
//	if (length != 0) {
//		result.x = pos.x / length;
//		result.y = pos.y / length;
//		result.z = pos.z / length;
//	}
//	return result;
//}
//float3 scale(float3 vec, float size) {
//	float3 result;
//	result.x = vec.x * size;
//	result.y = vec.y * size;
//	result.z = vec.z * size;
//	return result;
//}
//float3 subtract(float3 vec1, float3 vec2) {
//	float3 result;
//	result.x = vec1.x - vec2.x;
//	result.y = vec1.y - vec2.y;
//	result.z = vec1.z - vec2.z;
//	return result;
//}
//float distance(float3 vec1, float3 vec2) {
//	float3 dist = subtract(vec1, vec2);
//	float result = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.y * dist.y));
//	return result;
//}