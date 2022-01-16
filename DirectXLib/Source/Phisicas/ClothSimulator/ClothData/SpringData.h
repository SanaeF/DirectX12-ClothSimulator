#pragma once
#include <DirectXMath.h>

enum HIT_SPLIT {
	SINGLE=3,
	XYZ_ALL= (SINGLE * SINGLE * SINGLE) * 8//480
};
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
	bool is_no_add_vel;
};

struct SpringData {
	DirectX::XMFLOAT3 force;
	DirectX::XMFLOAT3 velocity;
	float mass;
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

struct PolygonModel {
	DirectX::XMINT3 id[6];
	int polygon_num;
};

struct ClothForce {//何も指定しないと、素朴な値が代入されます
	enum COLLISION_TYPE {
		OUT_STEP,//クロスのステップ内に含めない(低負荷)
		IN_STEP //クロスのステップ内に含める(高品質)
	};
	bool is_self_collision;//自己衝突を行うかどうか
	int collision_type;//当たり判定の種類を指定
	float collision_power;//当たり判定の攻撃力を指定
	float collision_size;
	float grid_mass;//質点の重さ
	float gravity;//重力
	float damping;//空気抵抗
	float dt;//速度の乗数
	float k;//バネ定数(指定する必要はない)
	Resistance tension;
	Resistance compress;
	Resistance share;
	Resistance bend;
	DirectX::XMFLOAT3 wind;//風力
	//風力
	void windF(float x, float y, float z) {
		wind.x = x;
		wind.y = y;
		wind.z = z;
	}
	//引張パラメーター(引数1:剛性、引数2:減衰)
	void tensionParam(float stretch, float shrink) {
		tension.stretch = stretch;
		tension.shrink = shrink;
	}
	//圧縮パラメーター(引数1:剛性、引数2:減衰)
	void compressParam(float stretch, float shrink) {
		compress.stretch = stretch;
		compress.shrink = shrink;
	}
	//せん断パラメーター(引数1:剛性、引数2:減衰)
	void shareParam(float stretch, float shrink) {
		share.stretch = stretch;
		share.shrink = shrink;
	}
	//曲げパラメーター(引数1:剛性、引数2:減衰)
	void bendParam(float stretch, float shrink) {
		bend.stretch = stretch;
		bend.shrink = shrink;
	}
	ClothForce() {
		is_self_collision = false;
		collision_type = COLLISION_TYPE::OUT_STEP;
		collision_power = 1.38f;
		collision_size=0.15;
		tension.stretch = 15;
		compress.stretch = 15;
		share.stretch = 15;
		bend.stretch = 5;
		tension.shrink = 5;
		compress.shrink = 5;
		share.shrink = 5;
		bend.shrink = 0.5;

		grid_mass = 1.f;
		gravity = 9.8;
		damping = 0;
		dt = 1.f;
		k = 1.2f;
		wind = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}
};