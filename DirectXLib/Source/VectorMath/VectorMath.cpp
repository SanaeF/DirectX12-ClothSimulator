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
	//‚È‚·Šp‚ÌŒvŽZ
	float VectorMath::subtendedAngle(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2) {
		float axb = pos1.x * pos2.x + pos1.y * pos2.y + pos1.z * pos2.z;
		float AxB =
			sqrt((pos1.x * pos1.x) + (pos1.y * pos1.y) + (pos1.z * pos1.z)) *
			sqrt((pos2.x * pos2.x) + (pos2.y * pos2.y) + (pos2.z * pos2.z));
		if (axb == 0 || AxB == 0)return acos(0);
		return acos(axb / AxB);
	}

	DirectX::XMFLOAT3 VectorMath::cross(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		result.x = p1.y * p2.z - p1.z * p2.y;
		result.y = p1.z * p2.x - p1.x * p2.z;
		result.z = p1.x * p2.y - p1.y * p2.x;
		return result;
	}
	DirectX::XMFLOAT3 VectorMath::createNormal(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) {
		DirectX::XMFLOAT3 p0_p1 = subtract(p1, p0);
		DirectX::XMFLOAT3 p0_p2 = subtract(p2, p0);
		float theta = subtendedAngle(p0_p1, p0_p2);
		return cross(p0_p1, p0_p2);
	}
	//2“_ŠÔ‚Ì’†“_
	DirectX::XMFLOAT3 VectorMath::slplitPoint(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		result.x = (p1.x + p2.x) / 2;
		result.y = (p1.y + p2.y) / 2;
		result.z = (p1.z + p2.z) / 2;
		return result;
	}
	float VectorMath::distance(
		DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2
	) {
		auto dist = subtract(vec1, vec2);
		auto result = sqrt((dist.x * dist.x) + (dist.y * dist.y) + (dist.z * dist.z));
		return result;
	}
	float VectorMath::distanceF2(DirectX::XMFLOAT2 vec1, DirectX::XMFLOAT2 vec2) {
		DirectX::XMFLOAT2 dist;
		dist.x = vec1.x - vec2.x;
		dist.y = vec1.y - vec2.y;
		auto result = sqrt((dist.x * dist.x) + (dist.y * dist.y));
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