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
		std::vector<lib::ModelParam> m_Pre_vertex;
		std::vector<SpringData>m_Spring_data;
		std::shared_ptr<ClothCollider>m_Collision;
		std::shared_ptr<StructuralSpring>m_Structural;
		std::shared_ptr<ShearSpring>m_Shear;
		std::shared_ptr<BendingSpring>m_Bending;
	public:
		SpringForceCalculator(std::vector<lib::ModelParam>& pre_vert);
		void gravity(int frame, std::vector<lib::ModelParam>& vertex, std::vector<std::vector<int>>& pre_index_id);
		void restriction(int frame, std::vector<lib::ModelParam>& vertex, std::vector<std::vector<int>>& pre_index_id);
		void createNewPosition(std::vector<lib::ModelParam>& vertex, std::vector<SpringData>& spring_data);
		void collision(std::vector<lib::ModelParam>& vertex);
		void setSpringForceData(std::vector<SpringData>& spring_data);
		std::vector<SpringData> getSpringForceData();
	private:
		bool isFixed(lib::ModelParam vert);
	};
}