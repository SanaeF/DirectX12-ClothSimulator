#include "VectorMath.h"
#include <iostream>

namespace lib {

	DirectX::XMFLOAT3 VectorMath::normalize(DirectX::XMFLOAT3 pos) {
		auto x = pos.x;
		auto y = pos.y;
		auto z = pos.z;
		auto length = mulAdd(pos, pos);
		DirectX::XMFLOAT3 result(0, 0, 0);
		if (length != 0) {
			result.x = pos.x / length;
			result.y = pos.y / length;
			result.z = pos.z / length;
		}
		return result;
	}
	DirectX::XMFLOAT3 VectorMath::add(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec1.x + vec2.x;
		result.y = vec1.y + vec2.y;
		result.z = vec1.z + vec2.z;
		return result;
	}
	DirectX::XMFLOAT3 VectorMath::scale(
		DirectX::XMFLOAT3 vec, float size
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec.x * size;
		result.y = vec.y * size;
		result.z = vec.z * size;
		return result;
	}
	DirectX::XMFLOAT3 VectorMath::subtract(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		DirectX::XMFLOAT3 result;
		result.x = vec1.x - vec2.x;
		result.y = vec1.y - vec2.y;
		result.z = vec1.z - vec2.z;
		return result;
	}
	DirectX::XMFLOAT3 VectorMath::uintVector(DirectX::XMFLOAT3 vec, float length) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		if (length != 0.f) {
			result.x = vec.x / length;
			result.y = vec.y / length;
			result.z = vec.z / length;
		}
		return result;
	}
	float VectorMath::distance(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		auto dist = subtract(vec1, vec2);
		auto result = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.y * dist.y));
		return result;
	}
	float VectorMath::mulAdd(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		auto x = vec1.x * vec2.x;
		auto y = vec1.y * vec2.y;
		auto z = vec1.z * vec2.z;
		float result = x + y + z;
		return sqrt(result);
	}
}