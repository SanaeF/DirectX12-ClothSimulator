#include "Vector3D.h"

namespace lib {
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
}