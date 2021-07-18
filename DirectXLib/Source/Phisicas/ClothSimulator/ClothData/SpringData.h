#pragma once
#include <DirectXMath.h>

struct SpringData {
	DirectX::XMFLOAT3 force;
	DirectX::XMFLOAT3 velocity;
	float mass;
};
struct Resistance {
	float stretch;
	float shrink;
};