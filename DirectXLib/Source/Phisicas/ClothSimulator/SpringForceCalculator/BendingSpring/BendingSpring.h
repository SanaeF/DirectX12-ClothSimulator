#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class BendingSpring {
	public:
		float bend; //曲がるのにどのくらい抵抗するか
		float bend_damping;//曲げ挙動時の減速量
	private:
		float m_Shrink;
		float m_Stretch;
	public:
		BendingSpring(float shrink, float stretch);
		void solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
		void solver2(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
	private:
		DirectX::XMFLOAT3 calcForce(
			lib::ModelVertex& vert1,
			lib::ModelVertex& vert2,
			float length,
			float constant
		);
	};
}