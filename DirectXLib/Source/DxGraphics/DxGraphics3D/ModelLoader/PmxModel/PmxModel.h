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
		std::vector<UINT>fixed_index;
		std::vector<UINT>same_position;
		std::vector<bool>is_same;
		std::vector<bool>is_input_same;
		std::vector<bool>is_standard;
	public:
		// PMXモデルデータのロード関数
		bool loadFile(const std::wstring& _filePath);
	private:
		void setStandardIndex(int id, lib::ModelData& data);
		//固定頂点のカラー割り当て
		void fixedIndex(lib::ModelData& data);
		//同一頂点のインデックスをまとめる
		void sameIndex(lib::ModelData& data);
		//インデックスをグループ化する
		void groupIndex(lib::ModelData& data);
	};
}