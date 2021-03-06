#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class StructuralSpring {
	public:
		//剛性
		float tension;//引っ張られるのにどのくらい抵抗するか
		float compression;//圧縮されるのにどのくらい抵抗するか
		//減衰
		float tension_damping;//引っ張られるときの減速量
		float compression_damping;//圧力時の減速量
		float damping;//減衰
	private:
		float m_Shrink;
		float m_Stretch;
	public:
		StructuralSpring(float shrink, float stretch);
		void solver(
			int vert_id,
			SpringData& spring_data,
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
		void solverLinear(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
	private:
		void tension_solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
		void compression_solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
		DirectX::XMFLOAT3 calcForce(
			lib::ModelVertex& vert1,
			lib::ModelVertex& vert2,
			float length,
			float constant
		);
	};
}