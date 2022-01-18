#pragma once
#include <DirectXMath.h>
#include <vector>
namespace lib {
	class VectorMath {
	public:
		static DirectX::XMFLOAT3 normalize(DirectX::XMFLOAT3 pos);
		static DirectX::XMFLOAT3 add(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
		static DirectX::XMFLOAT3 scale(DirectX::XMFLOAT3 vec, float size);
		static DirectX::XMFLOAT3 subtract(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
		static DirectX::XMFLOAT3 uintVector(DirectX::XMFLOAT3 vec, float length);
		static float subtendedAngle(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2);//‚È‚·Šp‚ÌŒvŽZ
		static DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		static DirectX::XMFLOAT3 createNormal(DirectX::XMFLOAT3 p0, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		//2“_ŠÔ‚Ì’†“_
		static DirectX::XMFLOAT3 slplitPoint(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		static float distance(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
		static float distanceF2(DirectX::XMFLOAT2 vec1, DirectX::XMFLOAT2 vec2);
		static float mulAdd(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
	};
}