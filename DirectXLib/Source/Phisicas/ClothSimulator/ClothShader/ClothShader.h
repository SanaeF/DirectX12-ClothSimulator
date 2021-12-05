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
			ClothForce& world_f,
			lib::ModelData& model,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<lib::ModelVertex>& last_vertex,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring_data
		);
	private:
		void massSpring(
			int model_id,
			int step,
			int time,
			ClothForce& world_f,
			lib::ModelData& model,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring_data
		);
		void collider(
			int model_id,
			int time,
			lib::ModelData& model,
			std::vector<MassModel>& mass_model,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<lib::ModelVertex>& last_vertex,
			std::vector<SpringData>& spring_data
		);

		void worldForce(int time, int step, ClothForce& world_f, lib::ModelData& model, std::vector<SpringData>& spring_data);
		void forceZero(std::vector<SpringData>& spring_data);
		bool isNullty(DirectX::XMFLOAT3 pos);
		bool isFixed(lib::ModelVertex vert);
	};
}