#pragma once
#include <vector>
#include "../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../ClothData/SpringData.h"
namespace phy {
	class ClothCPU {
	private:
	public:
		void execution(
			int step,
			int time,
			ClothForce& world_f,
			lib::ModelData& model,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<lib::ModelVertex>& last_vertex,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring_data,
			std::vector<PolygonModel>& polygon_model
		);
	private:
		bool isFixed(lib::ModelVertex vert);
		void worldForce(int time, int step, ClothForce& world_f, lib::ModelData& model, std::vector<SpringData>& spring_data);
	};
}