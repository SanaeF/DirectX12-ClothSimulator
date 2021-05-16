#pragma once
#include <DirectXMath.h>
#include <vector>
namespace lib {
	class Vector3D {
	public:
		DirectX::XMFLOAT3 add(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
		DirectX::XMFLOAT3 scale(DirectX::XMFLOAT3 vec, float size);
		DirectX::XMFLOAT3 subtract(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
	};
}