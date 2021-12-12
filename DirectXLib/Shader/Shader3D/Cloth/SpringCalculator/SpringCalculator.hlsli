
float mulAdd(float3 vec1, float3 vec2) {
	float x = vec1.x * vec2.x;
	float y = vec1.y * vec2.y;
	float z = vec1.z * vec2.z;
	float result = x + y + z;
	return sqrt(result);
}

float3 scale(float3 vec, float size) {
	float3 result;
	result.x = vec.x * size;
	result.y = vec.y * size;
	result.z = vec.z * size;
	return result;
}
float3 subtract(float3 vec1, float3 vec2) {
	float3 result = float3(0, 0, 0);
	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;
	return vec1- vec2;
}
float3 add(float3 vec1, float3 vec2) {
	float3 result;
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;
	return vec1 + vec2;
}
//Ç»Ç∑äpÇÃåvéZ
float subtendedAngle(float3 pos1, float3 pos2) {
	float axb = pos1.x * pos2.x + pos1.y * pos2.y + pos1.z * pos2.z;
	float AxB = 
		sqrt((pos1.x * pos1.x) + (pos1.y * pos1.y) + (pos1.z * pos1.z))*
		sqrt((pos2.x * pos2.x) + (pos2.y * pos2.y) + (pos2.z * pos2.z));
	if (axb == 0 || AxB == 0)return acos(0);
	return acos(axb / AxB);
}

float3 CalcForce(float3 pos1, float3 pos2, float length, float constant, float damping,float k) {
	float3 result = float3(0, 0, 0);
	float3 n = subtract(pos1, pos2);
	float leg = mulAdd(n, n);
	n = normalize(n);//ê≥ãKâª2
	float f = (length - leg) * (k/10);
	if (f < 0)f = f * (damping);//êLÇ—ÇÈí∑Ç≥Ç≈Ç†ÇÍÇŒå∏ëﬁ
	else f = f * (constant);
	result = scale(n, f);
	return result;
}

bool isFixed(float3 color) {
	bool result = false;
	if (color.x == 1.f &&
		color.y == 0.f &&
		color.z == 0.f)result = true;
	return result;
}

bool isNullData(float3 data) {
	bool result = false;
	if (data.x == 0.f &&
		data.y == 0.f &&
		data.z == 0.f)result = true;
	return result;
}