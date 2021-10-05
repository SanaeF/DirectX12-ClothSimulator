#pragma once
#include "../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
}
namespace phy {
	class ClothShader {
	private:
		std::shared_ptr<lib::DirectX12Manager> m_Dx12;
	public:
		ClothShader(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		void execution(
			int model_id,
			int step,
			int time,
			lib::ModelData& model,
			std::vector<SpringData>& spring_data,
			std::vector<std::vector<int>>& mass_spring_id
		);
	private:
		void massSpring(
			int model_id,
			int step,
			int time,
			lib::ModelData& model,
			std::vector<SpringData>& spring_data,
			std::vector<std::vector<int>>& mass_spring_id
		);
		void collider(
			int model_id,
			lib::ModelData& model,
			std::vector<SpringData>& spring_data,
			std::vector<std::vector<int>>& mass_spring_id
		);
		bool isNullty(DirectX::XMFLOAT3 pos);
	};
}