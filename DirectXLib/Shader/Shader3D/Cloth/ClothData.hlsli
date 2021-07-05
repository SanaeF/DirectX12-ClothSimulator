struct Vertex {
	float3 position;
	float4 normal;
	float3 color;
	float2 tex;
};
struct SpringData {
	float3 force;
	float3 velocity;
	float mass;
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