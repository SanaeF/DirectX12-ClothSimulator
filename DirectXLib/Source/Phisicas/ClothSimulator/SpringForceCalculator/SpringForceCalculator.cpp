#include "SpringForceCalculator.h"
#include "../../../VectorMath/VectorMath.h"
#include "ClothCollider/ClothCollider.h"
#include "StructuralSpring/StructuralSpring.h"
#include "ShearSpring/ShearSpring.h"
#include "BendingSpring/BendingSpring.h"
#include <thread>
namespace phy {
	SpringForceCalculator::SpringForceCalculator(std::vector<lib::ModelParam>& pre_vert):
		m_Pre_vertex(pre_vert)
	{
		m_Param.grid_mass = 1.f;
		m_Param.gravity = 9.8f;
		m_Param.damping = 0.10f;
		m_Param.dt = 0.036;
		m_Param.wind = 0.0f;
		m_Param.spring_constant = 15.f;
		m_Param.structural.shrink = 15.f;
		m_Param.structural.stretch = 5.f;
		m_Param.shear.shrink = 15.f;
		m_Param.shear.stretch = 5.f;
		m_Param.bending.shrink = 40.f;
		m_Param.bending.stretch = 0.f;
		m_Collision.reset(new ClothCollider(20, pre_vert.size()));
		m_Structural.reset(new StructuralSpring(15.f, 5.f));
		m_Shear.reset(new ShearSpring(15.f, 5.f));
		m_Bending.reset(new BendingSpring(40.f, 0.f));

		m_Structural->tension = 15;
		m_Structural->compression = 15;
		m_Shear->shear = 15;
		m_Bending->bend = 20;

		m_Structural->tension_damping = 25;
		m_Structural->compression_damping = 15;
		m_Shear->shear_damping = 15;
		m_Bending->bend = 3;
	}
	void SpringForceCalculator::gravity(
		int frame, std::vector<lib::ModelParam>& vertex, std::vector<std::vector<int>>& pre_index_id
	) {
		m_Spring_data.resize(vertex.size());
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto data = m_Spring_data[ite];
			data.force = DirectX::XMFLOAT3(0, 0, 0);
			data.mass = m_Param.grid_mass;
			//重力を加える
			data.force.y -= data.mass * m_Param.gravity;
			//風力を加える
			double r1 = frame /10;
			data.force.x = m_Param.wind * (sin(r1)* sin(r1) * 0.25 + 0.25);

			//ダンピング
			auto d = lib::VectorMath::scale(data.velocity, m_Param.damping);
			data.force = lib::VectorMath::subtract(data.force, d);
			m_Spring_data[ite] = data;
		}
	}
	void SpringForceCalculator::restriction(
		int frame,
		std::vector<lib::ModelParam>& vertex, 
		std::vector<std::vector<int>>& pre_index_id
	) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			/*m_Bending->solver2(ite, m_Spring_data, vertex, m_Pre_vertex, pre_index_id);
			m_Structural->solver2(ite, m_Spring_data, vertex, m_Pre_vertex, pre_index_id);
			m_Shear->solver2(ite, m_Spring_data, vertex, m_Pre_vertex, pre_index_id);*/
			//m_Bending->solver(ite, m_Spring_data, vertex, m_Pre_vertex, pre_index_id);
			m_Structural->solver(ite, m_Spring_data[ite], vertex, m_Pre_vertex, pre_index_id);
			m_Shear->solver(ite, m_Spring_data[ite], vertex, m_Pre_vertex, pre_index_id);
		}
		createNewPosition(vertex, m_Spring_data);
	}
	void SpringForceCalculator::createNewPosition(std::vector<lib::ModelParam>& vertex, std::vector<SpringData>& spring_data) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto v = lib::VectorMath::scale(spring_data[ite].force, 1 / (spring_data[ite].mass + spring_data[ite].mass));
			v = lib::VectorMath::scale(v, m_Param.dt);
			spring_data[ite].velocity = lib::VectorMath::add(spring_data[ite].velocity, v);
			v = lib::VectorMath::scale(spring_data[ite].velocity, m_Param.dt);
			vertex[ite].position = lib::VectorMath::add(vertex[ite].position, v);
			//力をゼロにする
			spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			//モデルに合わせて、あたり判定を空間を生成する
			//m_Collision->createSpaceBox(vertex[ite]);
		}
	}
	void SpringForceCalculator::collision(std::vector<lib::ModelParam>& vertex) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_Collision->spaceInput(ite, vertex[ite]);
		}
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_Collision->repulsion(ite, m_Spring_data[ite], vertex, m_Pre_vertex);
		}
	}
	void SpringForceCalculator::setSpringForceData(std::vector<SpringData>& spring_data) {
		m_Spring_data = spring_data;
	}
	std::vector<SpringData> SpringForceCalculator::getSpringForceData() {
		return m_Spring_data;
	}
	inline bool SpringForceCalculator::isFixed(lib::ModelParam vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}