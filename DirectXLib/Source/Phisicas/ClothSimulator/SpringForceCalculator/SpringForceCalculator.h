#pragma once
#include <vector>
#include <wrl.h>
#include <memory>
#include "../../../Graphics/Graph3D/Vertex/Vertex.h"
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
		Resistance structural;
		Resistance shear;
		Resistance bending;
	};
	class ClothCollider;
	class SpringForceCalculator {
	private:
		Paramater m_Param;
		std::vector<lib::Vertex> m_Pre_vertex;
		std::vector<SpringData>m_Spring_data;
		std::shared_ptr<ClothCollider>m_collision;
	public:
		SpringForceCalculator(std::vector<lib::Vertex> pre_vert);
		void gravity(int frame, std::vector<lib::Vertex> vertex, std::vector<std::vector<int>> pre_index_id);
		void step(int frame, std::vector<lib::Vertex> vertex, std::vector<std::vector<int>> pre_index_id);
		void createNewPosition(std::vector<lib::Vertex>& vertex);
		void collision(std::vector<lib::Vertex>& vertex);
		void setSpringForceData(std::vector<SpringData> spring_data);
		std::vector<SpringData> getSpringForceData();
	private:
		DirectX::XMFLOAT3 getSpringForce(
			lib::Vertex vert1,
			lib::Vertex vert2,
			float length,
			float constant,
			Resistance resistance
		);
		bool isFixed(lib::Vertex vert);
		void calcForce(
			std::vector<lib::Vertex> vertex,
			std::vector<lib::Vertex> pre_vertex,
			std::vector<std::vector<int>> pre_index_id
		);
	};
}