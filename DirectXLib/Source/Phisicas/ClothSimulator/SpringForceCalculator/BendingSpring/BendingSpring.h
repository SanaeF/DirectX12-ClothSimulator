#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class BendingSpring {
	private:
		float m_Shrink;
		float m_Stretch;
	public:
		BendingSpring(float shrink, float stretch);
		void solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelData>& vertex,
			std::vector<lib::ModelData>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
	private:
		DirectX::XMFLOAT3 calcForce(
			lib::ModelData& vert1,
			lib::ModelData& vert2,
			float length,
			float constant
		);
	};
}