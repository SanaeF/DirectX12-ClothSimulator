#pragma once
#include <vector>
#include <wrl.h>
#include "../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "ClothData/SpringData.h"
namespace phy {
	class ClothSimulator {
	private:
		std::vector<std::vector<int>> m_Pre_IndexID;
		static int m_time;
	public:
		ClothSimulator();
		//質点モデル-インデックスIDの生成用初期化
		ClothSimulator(std::vector<lib::ModelParam> vertex, std::vector<UINT> index, std::vector<std::vector<int>> index_group);
		static void resetPower(std::vector<SpringData>& spring_data);
		//物理演算の計算を更新
		static void update(
			std::vector<lib::ModelParam>& vertex,
			std::vector<UINT> index,
			std::vector<lib::ModelParam> pre_vertex,
			std::vector<std::vector<int>> pre_index_id,
			std::vector<SpringData>& spring_data
		);
		//質点モデル-インデックスIDの受け取り
		std::vector<std::vector<int>> getPreIndexID();
		~ClothSimulator();
	};
}