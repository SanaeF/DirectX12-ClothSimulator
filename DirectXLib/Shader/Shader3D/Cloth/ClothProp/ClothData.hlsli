struct Spring {
	float3 force;
	float3 velocity;
	float mass;
};
struct SpringData {
	Spring spring;
	float3 pos;
	float3 pre_pos;
	float3 col;
	int id0;
	int id1;
	int id2;
	int id3;
	int id4;
	int id5;
	int id6;
	int id7;
};

float3 sub(float3 vec1, float3 vec2) {
	float3 result;
	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;
	return result;
}
float3 add(float3 vec1, float3 vec2) {
	float3 result;
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;
	return result;
}
