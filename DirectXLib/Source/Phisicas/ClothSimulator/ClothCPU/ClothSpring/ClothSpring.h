#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class ClothSpring {
	private:
		ClothForce m_World_f;
		std::vector <lib::ModelVertex> m_Vert;
		std::vector<MassModel> m_Mass_model;
		std::vector<SpringData> m_Spring;
		std::vector<lib::ModelVertex> m_Pre_vert;
	public:
		ClothSpring(
			ClothForce& world_f,
			std::vector <lib::ModelVertex>& vert,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring,
			std::vector<lib::ModelVertex>& pre_vert
		);
		void execution(std::vector<SpringData>& spring);
	private:
		DirectX::XMFLOAT3 CalcForce(
			DirectX::XMFLOAT3 pos1,
			DirectX::XMFLOAT3 pos2,
			float length,
			float constant,
			float damping,
			float k
		);
		DirectX::XMFLOAT3 StructuralSolver(int id);
		DirectX::XMFLOAT3 CompressionSolver(int id);
		DirectX::XMFLOAT3 ShareSolver(int id);
		DirectX::XMFLOAT3 BendingSolver(int id);
		void executionPoint(int id);
		bool isFixed(lib::ModelVertex vert);
	};
}