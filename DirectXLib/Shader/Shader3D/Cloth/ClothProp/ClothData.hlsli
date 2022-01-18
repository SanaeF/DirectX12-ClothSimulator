struct Resistance {
	float stretch;
	float shrink;
};
struct SimulateParam {
	int vert_max;
	int time;
	float3 wind;
	Resistance tension;
	Resistance compress;
	Resistance share;
	Resistance bend;
	float dt;
	float k;
	float grid_mass;
	float gravity;
	float damping;
	bool is_no_add_vel;
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

struct PolygonModel {
	int3 id[6];
	int polygon_num;
};

struct MaxPosition {
	bool is_simulated;
	float3 max_pos;
	float3 min_pos;
};
