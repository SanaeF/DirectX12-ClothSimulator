struct CollisionParam {
	int vertex_size;
	float power;
};

struct SpaceData {
	int count;
	int id[216];
};

struct SendParam {
	int vertex_size;
	int split_num;
	float3 max_pos;
	float3 min_pos;
	float3 zero_pos;
};