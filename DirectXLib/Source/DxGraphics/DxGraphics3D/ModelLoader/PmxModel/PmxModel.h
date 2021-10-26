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
		std::vector<UINT>m_Fixed_idx;
		std::vector<UINT>m_Same_pos;
		std::vector<bool>m_Is_same;
		std::vector<bool>m_Is_input_same;
		std::vector<bool>m_Is_standard;
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
		//文字列の読み込み
		bool loadPMXStringUTF16(std::ifstream& _file, std::wstring& output, std::wstring& path);
	};
}