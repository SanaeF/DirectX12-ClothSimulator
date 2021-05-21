#include "Vector3D.h"
#include <iostream>

namespace lib {
	DirectX::XMFLOAT3 Vector3D::normalize(DirectX::XMFLOAT3 pos) {
		auto x = pos.x;
		auto y = pos.y;
		auto z = pos.z;
		auto length = x * x + y * y + z * z;
		if (length > 0)length = 1 / sqrt(length);
		DirectX::XMFLOAT3 result;
		result.x = pos.x * length;
		result.y = pos.y * length;
		result.z = pos.z * length;
		return result;
	}
	DirectX::XMFLOAT3 Vector3D::add(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec1.x + vec2.x;
		result.y = vec1.y + vec2.y;
		result.z = vec1.z + vec2.z;
		return result;
	}
	DirectX::XMFLOAT3 Vector3D::scale(
		DirectX::XMFLOAT3 vec, float size
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec.x * size;
		result.y = vec.y * size;
		result.z = vec.z * size;
		return result;
	}
	DirectX::XMFLOAT3 Vector3D::subtract(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec1.x - vec2.x;
		result.y = vec1.y - vec2.y;
		result.z = vec1.z - vec2.z;
		return result;
	}
	float Vector3D::distance(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		auto dist = subtract(vec1, vec2);
		auto result = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.y * dist.y));
		return result;
	}
}