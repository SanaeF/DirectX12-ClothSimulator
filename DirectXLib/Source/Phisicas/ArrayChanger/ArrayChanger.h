#pragma once
#include <DirectXMath.h>

namespace phy {
	template<class STR>
	class ArrayChanger {
	public:
		static STR* Float3toArray3(DirectX::XMFLOAT3 input);
		static DirectX::XMFLOAT3 Array3toFloat3(STR* input);
	};
	//typedef ChangeArray<int> int3;
	//typedef ChangeArray<float> Float3x3;
}