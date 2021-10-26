struct SimulateParam {
	int vert_max;
	float gravity;
	float dt;
};

struct SpringData {
	float3 force;
	float3 velocity;
	float mass;
};

struct MassModel {
	int id0;
	int id1;
	int id2;
	int id3;
	int id4;
	int id5;
	int id6;
	int id7;
	int id8;
	int id9;
	int id10;
	int id11;
};
struct ModelVertex {
	float3 pos;
	float4 normal;
	float2 tex;
	float3 color;
};
struct MaxPosition {
	bool is_simulated;
	float3 max_pos;
	float3 min_pos;
};
