#include "ArrayChanger.h"
namespace phy {
	template<class STR>
	STR* ArrayChanger<STR>::Float3toArray3(DirectX::XMFLOAT3 input) {
		STR result[3];
		result[0] = input.x;
		result[1] = input.y;
		result[2] = input.z;
		return result;
	}
	template<class STR>
	DirectX::XMFLOAT3 ArrayChanger<STR>::Array3toFloat3(STR* input) {
		DirectX::XMFLOAT3 result;
		result.x = input[0];
		result.y = input[1];
		result.z = input[2];
		return result;
	}
}