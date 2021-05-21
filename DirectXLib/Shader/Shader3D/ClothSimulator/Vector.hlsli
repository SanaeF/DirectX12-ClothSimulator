struct Vector {
	float3 normalize(float3 pos) {
		float x = pos.x;
		float y = pos.y;
		float z = pos.z;
		float length = mul(x, x) + mul(y, y) + mul(z, z);
		if (length > 0)length = 1 / sqrt(length);
		float3 result;
		result.x = mul(pos.x, length);
		result.y = mul(pos.y, length);
		result.z = mul(pos.z, length);
		return result;
	}
	float3 add(float3 vec1, float3 vec2) {
		float3 result;
		result.x = vec1.x + vec2.x;
		result.y = vec1.y + vec2.y;
		result.z = vec1.z + vec2.z;
		return result;
	}
	float3 scale(float3 vec, float size) {
		float3 result;
		result.x = vec.x * size;
		result.y = vec.y * size;
		result.z = vec.z * size;
		return result;
	}
	float3 sub(float3 vec1, float3 vec2) {
		float3 result;
		result.x = vec1.x - vec2.x;
		result.y = vec1.y - vec2.y;
		result.z = vec1.z - vec2.z;
		return result;
	}
	float dist(float3 vec1, float3 vec2) {
		return distance(vec1, vec2);
	}
};