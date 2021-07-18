#include "SpringForceCalculator.h"
#include "../../../VectorMath/VectorMath.h"
#include "ClothCollider/ClothCollider.h"
#include "StructuralSpring/StructuralSpring.h"
#include "ShearSpring/ShearSpring.h"
#include "BendingSpring/BendingSpring.h"
#include <thread>
namespace phy {
	SpringForceCalculator::SpringForceCalculator(std::vector<lib::ModelData> pre_vert):
		m_Pre_vertex(pre_vert)
	{
		m_Param.grid_mass = 1.f;
		m_Param.gravity = 9.8f;
		m_Param.damping = 0.9f;
		m_Param.dt = 0.01;
		m_Param.wind = 25.f;
		m_Param.spring_constant = 15.f;
		m_Param.structural.shrink = 15.f;
		m_Param.structural.stretch = 5.f;
		m_Param.shear.shrink = 15.f;
		m_Param.shear.stretch = 5.f;
		m_Param.bending.shrink = 40.f;
		m_Param.bending.stretch = 0.f;
		m_collision.reset(new ClothCollider(40, pre_vert.size()));
		m_Structural.reset(new StructuralSpring(15.f, 5.f));
		m_Shear.reset(new ShearSpring(15.f, 5.f));
		m_Bending.reset(new BendingSpring(40.f, 0.f));
	}
	void SpringForceCalculator::gravity(
		int frame, std::vector<lib::ModelData>& vertex, std::vector<std::vector<int>> pre_index_id
	) {
		m_Spring_data.resize(vertex.size());
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto data = m_Spring_data[ite];
			data.mass = m_Param.grid_mass;
			//力をゼロにする
			data.force = DirectX::XMFLOAT3(0, 0, 0);
			//重力を加える
			data.force.y -= data.mass * m_Param.gravity;
			//風力を加える
			double r1 = frame /10;
			data.force.x = m_Param.wind;data.force.x += m_Param.wind * (sin(r1) * 1.5 + 0.5);

			//ダンピング
			auto d = lib::VectorMath::scale(data.velocity, m_Param.damping);
			data.force = lib::VectorMath::subtract(data.force, d);
			m_Spring_data[ite] = data;
		}
	}
	void SpringForceCalculator::restriction(
		int frame,
		std::vector<lib::ModelData>& vertex, 
		std::vector<std::vector<int>> pre_index_id
	) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_Structural->solver(ite, m_Spring_data[ite], vertex, m_Pre_vertex, pre_index_id);
			m_Shear->solver(ite, m_Spring_data[ite], vertex, m_Pre_vertex, pre_index_id);
			m_Bending->solver(ite, m_Spring_data, vertex, m_Pre_vertex, pre_index_id);
		}
		//calcForce(vertex, m_Pre_vertex, pre_index_id);
		createNewPosition(vertex);
	}
	void SpringForceCalculator::createNewPosition(std::vector<lib::ModelData>& vertex) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto v = lib::VectorMath::scale(m_Spring_data[ite].force, 1 / (m_Spring_data[ite].mass+ m_Spring_data[ite].mass));
			v = lib::VectorMath::scale(v, m_Param.dt);
			m_Spring_data[ite].velocity = lib::VectorMath::add(m_Spring_data[ite].velocity, v);
			v = lib::VectorMath::scale(m_Spring_data[ite].velocity, m_Param.dt);
			vertex[ite].position = lib::VectorMath::add(vertex[ite].position, v);
			//モデルに合わせて、あたり判定を空間を生成する
			m_collision->createSpaceBox(vertex[ite]);
		}
	}
	void SpringForceCalculator::collision(std::vector<lib::ModelData>& vertex) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_collision->spaceInput(ite, vertex[ite]);
		}
		auto new_data = vertex;
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			new_data[ite] = m_collision->repulsion(ite, vertex, m_Pre_vertex);
		}
		vertex = new_data;
	}
	inline void SpringForceCalculator::calcForce(
		std::vector<lib::ModelData> vertex,
		std::vector<lib::ModelData> pre_vertex,
		std::vector<std::vector<int>> pre_index_id
	) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_Structural->solver(ite, m_Spring_data[ite], vertex, pre_vertex, pre_index_id);
			auto id = ite;
			for (int ite2 = 0; ite2 < 4; ite2++) {
				auto id2 = pre_index_id[ite][4 + ite2];
				if (id2 == -1)continue;
				float Natulength = lib::VectorMath::distance(pre_vertex[id].position, pre_vertex[id2].position);//ばねの自然長
				auto f = getSpringForce(vertex[id], vertex[id2], Natulength, 125, m_Param.shear, m_Spring_data[ite]);
				m_Spring_data[ite].force = lib::VectorMath::add(m_Spring_data[ite].force, f);
			}
			for (int ite2 = 0; ite2 < 4; ite2++) {
				auto id2 = pre_index_id[ite][4 + ite2];
				if (id2 == -1)continue;
				for (int ite3 = 0; ite3 < 4; ite3++) {
					auto id3 = pre_index_id[ite][4 + ite3];
					if (id3 == -1 || id2 == id3)continue;
					float Natulength = lib::VectorMath::distance(pre_vertex[id2].position, pre_vertex[id3].position);//ばねの自然長
					auto f = getSpringForce(vertex[id2], vertex[id3], Natulength, 15, m_Param.bending, m_Spring_data[id2]);
					//m_Spring_data[id2].force = lib::VectorMath::add(m_Spring_data[id2].force, f);
				}
			}
			//auto f = getSpringForce(vertex[id], pre_vertex[id], 0, 150, m_Param.structural, m_Spring_data[ite]);
			//m_Spring_data[ite].force = lib::VectorMath::add(m_Spring_data[ite].force, f);
		}
	}
	inline DirectX::XMFLOAT3
		SpringForceCalculator::getSpringForce(
			lib::ModelData vert1,
			lib::ModelData vert2,
			float length,
			float constant,
			Resistance resistance,
			SpringData spring_data
		) {
		auto n = lib::VectorMath::subtract(vert1.position, vert2.position);
		auto leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//正規化2
		float f = (length - leg) * constant;
		f >= 0 ? f *= resistance.shrink : f *= resistance.stretch;
		return lib::VectorMath::scale(n, f);
	}
	void SpringForceCalculator::setSpringForceData(std::vector<SpringData> spring_data) {
		m_Spring_data = spring_data;
	}
	std::vector<SpringData> SpringForceCalculator::getSpringForceData() {
		return m_Spring_data;
	}
	inline bool SpringForceCalculator::isFixed(lib::ModelData vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}