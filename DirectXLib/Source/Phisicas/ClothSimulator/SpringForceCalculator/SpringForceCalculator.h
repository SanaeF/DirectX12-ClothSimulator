#pragma once
#include <vector>
#include <wrl.h>
#include <memory>
#include "../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../ClothData/SpringData.h"
namespace phy {
	struct Resistance {
		float stretch;
		float shrink;
	};
	struct Paramater {
		float grid_mass;
		float gravity;
		float damping;
		float dt;
		float wind;
		float spring_constant;
		Resistance structural;
		Resistance shear;
		Resistance bending;
	};
	class ClothCollider;
	class StructuralSpring;
	class ShearSpring;
	class BendingSpring;
	class SpringForceCalculator {
	private:
		Paramater m_Param;
		std::vector<lib::ModelData> m_Pre_vertex;
		std::vector<SpringData>m_Spring_data;
		std::shared_ptr<ClothCollider>m_collision;
		std::shared_ptr<StructuralSpring>m_Structural;
		std::shared_ptr<ShearSpring>m_Shear;
		std::shared_ptr<BendingSpring>m_Bending;
	public:
		SpringForceCalculator(std::vector<lib::ModelData> pre_vert);
		void gravity(int frame, std::vector<lib::ModelData>& vertex, std::vector<std::vector<int>> pre_index_id);
		void restriction(int frame, std::vector<lib::ModelData>& vertex, std::vector<std::vector<int>> pre_index_id);
		void createNewPosition(std::vector<lib::ModelData>& vertex);
		void collision(std::vector<lib::ModelData>& vertex);
		void setSpringForceData(std::vector<SpringData> spring_data);
		std::vector<SpringData> getSpringForceData();
	private:
		DirectX::XMFLOAT3 getSpringForce(
			lib::ModelData vert1,
			lib::ModelData vert2,
			float length,
			float constant,
			Resistance resistance,
			SpringData spring_data
		);
		bool isFixed(lib::ModelData vert);
		void calcForce(
			std::vector<lib::ModelData> vertex,
			std::vector<lib::ModelData> pre_vertex,
			std::vector<std::vector<int>> pre_index_id
		);
	};
}