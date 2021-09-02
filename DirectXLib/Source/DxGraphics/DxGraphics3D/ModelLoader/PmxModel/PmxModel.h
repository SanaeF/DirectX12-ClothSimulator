#pragma once
#include <d3dx12.h>
#include <string>
#include <memory>
#include <vector>
#include "../../ModelData/ModelData.h"

namespace lib {
	class DirectX12Manager;
}
namespace model {
	class PmxModel {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:
		// PMXモデルデータのロード関数
		bool loadFile(const std::wstring& _filePath);
	};
}