#include "SpringForceCalculator.h"
#include "../../../VectorMath/VectorMath.h"
#include "ClothCollider/ClothCollider.h"
#include <thread>
namespace phy {
	SpringForceCalculator::SpringForceCalculator(std::vector<lib::Vertex> pre_vert):
		m_Pre_vertex(pre_vert)
	{
		m_Param.grid_mass = 1.f;
		m_Param.gravity = 9.8f;
		m_Param.damping = 4.5f;
		m_Param.dt = 0.01;
		m_Param.wind = 25.f;
		m_Param.structural.shrink = 15.f;
		m_Param.structural.stretch = 5.f;
		m_Param.shear.shrink = 15.f;
		m_Param.shear.stretch = 5.f;
		m_Param.bending.shrink = 40.f;
		m_Param.bending.stretch = 0.f;
		m_collision.reset(new ClothCollider(40, pre_vert.size()));
	}
	void SpringForceCalculator::gravity(
		int frame, std::vector<lib::Vertex> vertex, std::vector<std::vector<int>> pre_index_id
	) {
		m_Spring_data.resize(vertex.size());
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto data = m_Spring_data[ite];
			data.mass = m_Param.grid_mass;
			//�͂��[���ɂ���
			//data.force = DirectX::XMFLOAT3(0, 0, 0);
			//�d�͂�������
			data.force.y -= data.mass * m_Param.gravity;
			//���͂�������
			double r1 = frame /10;
			//data.force.x += m_Param.wind * (sin(r1) * 1.5 + 0.5);

			//�_���s���O
			auto d = lib::VectorMath::scale(data.velocity, m_Param.damping);
			data.force = lib::VectorMath::subtract(data.force, d);
			m_Spring_data[ite] = data;
		}
	}
	void SpringForceCalculator::step(
		int frame,
		std::vector<lib::Vertex> vertex, 
		std::vector<std::vector<int>> pre_index_id
	) {
		calcForce(vertex, m_Pre_vertex, pre_index_id);
	}
	void SpringForceCalculator::createNewPosition(std::vector<lib::Vertex>& vertex) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto v = lib::VectorMath::scale(m_Spring_data[ite].force, 1 / m_Spring_data[ite].mass);
			v = lib::VectorMath::scale(v, m_Param.dt);
			m_Spring_data[ite].velocity = lib::VectorMath::add(m_Spring_data[ite].velocity, v);
			v = lib::VectorMath::scale(m_Spring_data[ite].velocity, m_Param.dt);
			vertex[ite].position = lib::VectorMath::add(vertex[ite].position, v);
			//�͂��[���ɂ���
			m_Spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			//���f���ɍ��킹�āA�����蔻�����Ԃ𐶐�����
			//m_collision->createSpaceBox(vertex[ite]);
		}
	}
	void SpringForceCalculator::collision(std::vector<lib::Vertex>& vertex) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_collision->spaceInput(ite, vertex[ite]);
		}
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			m_collision->repulsion(ite, vertex, m_Pre_vertex);
		}
	}
	inline void SpringForceCalculator::calcForce(
		std::vector<lib::Vertex> vertex,
		std::vector<lib::Vertex> pre_vertex,
		std::vector<std::vector<int>> pre_index_id
	) {
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (isFixed(vertex[ite]))continue;
			auto id = ite;
			for (int ite2 = 0; ite2 < 4; ite2++) {
				auto id2 = pre_index_id[ite][ite2];
				if (id2 == -1)continue;
				float Natulength = lib::VectorMath::distance(pre_vertex[id].position, pre_vertex[id2].position);//�΂˂̎��R��
				auto f = getSpringForce(vertex[id], vertex[id2], Natulength, 150, m_Param.structural);
				m_Spring_data[ite].force = lib::VectorMath::add(m_Spring_data[ite].force, f);
			}
			for (int ite2 = 0; ite2 < 4; ite2++) {
				auto id2 = pre_index_id[ite][4 + ite2];
				if (id2 == -1)continue;
				float Natulength = lib::VectorMath::distance(pre_vertex[id].position, pre_vertex[id2].position);//�΂˂̎��R��
				auto f = getSpringForce(vertex[id], vertex[id2], Natulength, 125, m_Param.shear);
				m_Spring_data[ite].force = lib::VectorMath::add(m_Spring_data[ite].force, f);
			}

			//for (int ite2 = 0; ite2 < 4; ite2++) {
			//	auto id2 = pre_index_id[ite][8 + ite2];
			//	if (id2 == -1)continue;
			//	float Natulength = lib::VectorMath::distance(pre_vertex[id].position, pre_vertex[id2].position);//�΂˂̎��R��
			//	auto f = getSpringForce(vertex[id], vertex[id2], Natulength, 100, m_Param.bending);
			//	m_Spring_data[ite].force = lib::VectorMath::add(m_Spring_data[ite].force, f);
			//}
		}
	}
	inline DirectX::XMFLOAT3
		SpringForceCalculator::getSpringForce(
			lib::Vertex vert1,
			lib::Vertex vert2,
			float length,
			float constant,
			Resistance resistance
		) {
		auto n = lib::VectorMath::subtract(vert1.position, vert2.position);
		auto leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//���K��2
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
	inline bool SpringForceCalculator::isFixed(lib::Vertex vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}