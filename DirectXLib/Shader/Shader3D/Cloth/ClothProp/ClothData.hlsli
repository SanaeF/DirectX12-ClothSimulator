struct Resistance {
	float stretch;
	float shrink;
};
struct SimulateParam {
	int vert_max;
	Resistance tension;
	Resistance compress;
	Resistance share;
	Resistance bend;
	float dt;
	float k;
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
struct MaxPosition {
	bool is_simulated;
	float3 max_pos;
	float3 min_pos;
};
