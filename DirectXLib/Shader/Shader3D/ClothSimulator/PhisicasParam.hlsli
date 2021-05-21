struct Spring {
	float constant;
	float stretch;
	float shrink;
};

struct PhisicasParam{
	float3 force3;
	float force2;
	float gravity;
	float resistance;
	float relaxation;
	float mass;
	float wind;
	float acc;
	Spring spring;
};

bool between(float param, float deltaP1, float deltaP2) {
	if (deltaP1 <= param && param < deltaP2)return true;
	return false;
}