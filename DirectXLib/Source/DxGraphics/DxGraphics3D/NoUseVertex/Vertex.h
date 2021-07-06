#pragma once
#include <DirectXMath.h>

namespace lib {
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT2 tex;
	};
}
