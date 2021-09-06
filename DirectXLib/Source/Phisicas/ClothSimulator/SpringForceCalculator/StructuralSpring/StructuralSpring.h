#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class StructuralSpring {
	public:
		//„«
		float tension;//ˆø‚Á’£‚ç‚ê‚é‚Ì‚É‚Ç‚Ì‚­‚ç‚¢’ïR‚·‚é‚©
		float compression;//ˆ³k‚³‚ê‚é‚Ì‚É‚Ç‚Ì‚­‚ç‚¢’ïR‚·‚é‚©
		//Œ¸Š
		float tension_damping;//ˆø‚Á’£‚ç‚ê‚é‚Æ‚«‚ÌŒ¸‘¬—Ê
		float compression_damping;//ˆ³—Í‚ÌŒ¸‘¬—Ê
		float damping;//Œ¸Š
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
		void solver2(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelParam>& vertex,
			std::vector<lib::ModelParam>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
	private:
		void tension_solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelParam>& vertex,
			std::vector<lib::ModelParam>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
		void compression_solver(
			int vert_id,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelParam>& vertex,
			std::vector<lib::ModelParam>& pre_vert,
			std::vector<std::vector<int>>& pre_index_id
		);
		DirectX::XMFLOAT3 calcForce(
			lib::ModelParam& vert1,
			lib::ModelParam& vert2,
			float length,
			float constant
		);
	};
}