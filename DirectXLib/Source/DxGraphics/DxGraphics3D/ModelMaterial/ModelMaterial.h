#pragma once
#include "../ModelData/ModelData.h"
namespace lib {
	class ModelMaterial {
	private:
		struct MaterialForHlsl {
			DirectX::XMFLOAT3 diffuse; //ディフューズ色
			float alpha; // ディフューズα
			DirectX::XMFLOAT3 specular; //スペキュラ色
			float specularity;//スペキュラの強さ(乗算値)
			DirectX::XMFLOAT3 ambient; //アンビエント色
		};
	public:
	private:
	};
}