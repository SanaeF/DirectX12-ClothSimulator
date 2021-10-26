#pragma once
#include <DirectXMath.h>

enum HIT_SPLIT {
	SINGLE=3,
	XYZ_ALL= (SINGLE * SINGLE * SINGLE) * 8//480
};

struct SimulateParam {
	int vert_max;
	/*float gravity;
	float dt;*/
};

struct SpringData {
	DirectX::XMFLOAT3 force;
	DirectX::XMFLOAT3 velocity;
	float mass;
};
struct Resistance {
	float stretch;
	float shrink;
};

struct ClothData {
	SpringData spring;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 pre_pos;
	DirectX::XMFLOAT3 color;
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
	bool simulate;
	int vertex_size;
	int step;
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


struct WorldForce {
	float grid_mass;
	float gravity;
	float damping;
	float dt;
	DirectX::XMFLOAT3 wind;
};