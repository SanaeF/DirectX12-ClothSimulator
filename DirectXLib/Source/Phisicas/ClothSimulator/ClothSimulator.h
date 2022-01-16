#pragma once
#include <vector>
#include <memory>
#include <wrl.h>
#include "../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "ClothData/SpringData.h"

namespace lib {
	class DirectX12Manager;
	struct ClothHandleData {
		int step;
		std::vector<ModelVertex>pre_vert;
		std::vector<ModelVertex>last_vert;
		std::vector<std::vector<int>>mass_springs;
		std::vector<PolygonModel>polygon_model;
		std::vector<MassModel>mass_model;
		std::vector<SpringData>spring_data;
		ClothForce world_f;
	};
}
namespace phy {
	class ClothSimulator {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::vector<std::vector<int>> m_Pre_IndexID;
		static std::vector<lib::ClothHandleData> m_Cloth_handle_data;
		static std::vector<std::vector<ClothData>> m_ClothData;
		static int m_time;
	public:
		ClothSimulator();
		//モデルから得られるシミュレート用の情報を設定
		static void initialize(int handle, int step, lib::ModelData& model, ClothForce world_f);
		static void worldForce(int handle, ClothForce world_f);
		static void resetPower(std::vector<SpringData>& spring_data);
		//物理演算の計算を更新
		static void update(
			lib::ModelData& model,
			std::vector<SpringData>& spring_data,
			std::vector<std::vector<int>>& mass_spring_id
		);
		static void execution(
			int handle,
			lib::ModelData& model,
			std::shared_ptr<lib::DirectX12Manager>& dx_12
		);
		static void resetModel(int handle, lib::ModelData& model);
		~ClothSimulator();
	private:
	};
}