#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class StructuralSpring {
	private:
		float m_Shrink;
		float m_Stretch;
	public:
		StructuralSpring(float shrink, float stretch);
		void solver(
			int vert_id,
			SpringData& spring_data,
			std::vector<lib::ModelParam>& vertex,
			std::vector<lib::ModelParam>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
	private:
		DirectX::XMFLOAT3 calcForce(
			lib::ModelParam& vert1,
			lib::ModelParam& vert2,
			float length,
			float constant
		);
	};
}