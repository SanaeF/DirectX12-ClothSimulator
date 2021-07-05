#include "ClothSimulatorOld.h"
#include <cassert>
#include "../../DirectX12/DirectXLib/Source/DirectX12/DirectX12Manager.h"
#include "../Physics/Physics.h"
#include "../Vector3D/Vector3D.h"
#include "ClothShader/ClothShader.h"
#include <thread>
#include <cassert>
namespace lib {
	ClothSimulatorOld::ClothSimulatorOld(
		ComPtr<ID3D12Device> device,
		std::vector<Vertex> vertex,
		std::vector<UINT> index
	) {
		m_Phys.reset(new physlib::Physics());
		m_Shader.reset(new compute::ClothShader(device, vertex, index));
		weigth.resize(vertex.size());
		velocity.resize(vertex.size());
		for (int ite = 0; ite < weigth.size(); ite++) {
			weigth[ite] = 1.00f;
		}
		m_Param.damping = 0.25;
		m_Phys->param.mass = 1.f;
		m_Phys->param.gravity = 9.8f;
		m_Phys->param.resistance = 0.5f;
		m_Phys->param.acc = 0.f;
		m_Phys->param.wind = 25.f;
		m_Phys->param.spring.constant = 0.25f;
		m_Phys->param.spring.structural.shrink = 15.f;
		m_Phys->param.spring.structural.stretch = 5.f;
		m_Phys->param.spring.shear.shrink = 15.f;
		m_Phys->param.spring.shear.stretch = 5.f;
		m_Phys->param.spring.bending.shrink = 40.f;
		m_Phys->param.spring.bending.stretch = 0.f;
	/*	m_Phys->param.force3.x = 0.1;
		m_Phys->param.force3.y = 0.1;
		m_Phys->param.force3.z = 0.1;*/
	}
	void ClothSimulatorOld::inputParamater() {
	}
	void ClothSimulatorOld::calculate(
		int step, std::shared_ptr<DirectX12Manager> dx12,
		std::vector<Vertex>& vertex,
		std::vector<Vertex> pre_vertex,
		std::vector<Vertex>* vir_vertex,
		std::vector<UINT>& index,
		std::vector<int>* pre_index
	) {//コイツはシェーダー
		/*mShader->ShaderCalculater(dx12);
		vertex = mShader->getSimulatedData().vertex;
		index = mShader->getSimulatedData().index;
		return;*/
		m_EquivalentIndex.resize(vertex.size());
		m_Phys->param.acc+=step/10;
		multiCalculate(4, 0, step, vertex, pre_vertex, vir_vertex, index, pre_index);
	}
	void ClothSimulatorOld::multiCalculate(
		int th_num, int id, int step,
		std::vector<Vertex>& vertex,
		std::vector<Vertex> pre_vertex,
		std::vector<Vertex>* vir_vertex,
		std::vector<UINT>& index,
		std::vector<int>* per_index
	) {
		if (th_num <= id)return;
		auto vert_num = vertex.size();
		auto vert = vertex;
		auto ind = index;
		auto per_vert = pre_vertex;
		auto per_ind = per_index;



		for (int vNum = 0; vNum < vert_num; vNum++) {
			if (vert[vNum].color.x == 1.f &&
				vert[vNum].color.y == 0.f &&
				vert[vNum].color.z == 0.f)continue;
			auto grav = m_Phys->param.gravity * m_Phys->param.mass;
			auto wind = m_Phys->param.wind * m_Phys->param.mass;
			auto spr_const = m_Phys->param.spring.constant * m_Phys->param.mass;
			auto force = m_Phys->param.force3;
			//　重力
			force.y -= m_Phys->param.gravity;
			//　風
			force.x += m_Phys->param.wind * (sin(m_Phys->param.acc) * 0.5 + 0.5);
			force = lib::Vector3D::scale(force, step * step * 0.5 / m_Phys->param.mass);
			auto resi = 1.f - m_Phys->param.resistance * step;

			// 変位
			DirectX::XMFLOAT3 dtX = lib::Vector3D::subtract(vert[vNum].position, vert[vNum].position);
			dtX = lib::Vector3D::add(dtX, force);
			dtX = lib::Vector3D::scale(dtX, resi);
			// 位置更新
			dtX = lib::Vector3D::scale(dtX, weigth[vNum]);
			vert[vNum].position = lib::Vector3D::add(vert[vNum].position, dtX);
		}
		for (int vNum = 0; vNum < vert_num; vNum++) {
			if (vert[vNum].color.x == 1.f &&
				vert[vNum].color.y == 0.f &&
				vert[vNum].color.z == 0.f) {
				continue;
			}
			//auto spring_f = force;
			//auto vel = lib::Vector3D::scale(velocity[vNum], m_Param.damping);
			//spring_f = lib::Vector3D::subtract(spring_f, vel);
			for (int ite = 0; ite < step; ite++) {
				//構造ばね
				for (int ite2 = 0; ite2 < 4; ite2++) {
					auto id = per_ind[ite2][vNum];
					if (id >= vert_num)continue;
					//auto f = physlib::Physics::getSpringForce3(500, vert[id].position, vert[vNum].position, per_vert[id].position, per_vert[vNum].position);
					float Natulength = lib::Vector3D::distance(per_vert[vNum].position, per_vert[id].position);//ばねの自然長
					//auto f = getSpringForce(vert[vNum], vert[id], Natulength, 500);
					vir_vertex[0][vNum].position = spring(id, step, vNum, vert[vNum], vir_vertex[ite][vNum], Natulength,
						m_Phys->param.spring.structural.shrink, m_Phys->param.spring.structural.stretch);
				}
				//せん断ばね
				for (int ite2 = 0; ite2 < 4; ite2++) {
					auto id = per_ind[4+ite2][vNum];
					if (id >= vert_num)continue;
					//auto f = physlib::Physics::getSpringForce3(250, vert[vNum].position, vert[id].position, per_vert[vNum].position, per_vert[id].position);
					float Natulength = lib::Vector3D::distance(per_vert[vNum].position, per_vert[id].position);//ばねの自然長
					//auto f = getSpringForce(vert[vNum], vert[id], Natulength, 250);
					vir_vertex[0][vNum].position = spring(id, step, vNum, vert[vNum], vir_vertex[5+ite][vNum], Natulength,
						m_Phys->param.spring.shear.shrink, m_Phys->param.spring.shear.stretch);
				}
				////曲げばね
				//for (int ite2 = 0; ite2 < 4; ite2++) {
				//	auto id = per_ind[ite2][vNum];
				//	if (id >= vert_num)continue;
				//	float Natulength = lib::Vector3D::distance(per_vert[vNum].position, per_vert[id].position);//ばねの自然長
				//	lib::Vertex p2;
				//	p2.position = lib::Vector3D::scale(vir_vertex[ite2 + 1][vNum].position, Natulength);
				//	p2.color = vir_vertex[ite2 + 1][vNum].color;
				//	vir_vertex[0][vNum].position = spring(id, step, vNum, vir_vertex[0][vNum], p2, Natulength,
				//		m_Phys->param.spring.bending.shrink, m_Phys->param.spring.bending.stretch);
				//}
			}
			vert[vNum] = vir_vertex[0][vNum];
		}
		vertex = vert;
	}
	DirectX::XMFLOAT3 ClothSimulatorOld::getSpringForce(
		Vertex vert1,
		Vertex vert2,
		float length,
		float constant
	) {
		auto dist = lib::Vector3D::distance(vert1.position, vert2.position);//現在のばねの長さ
		auto x = dist - length;//ばねの伸び縮みの長さ(現在の長さ-自然長い)
		auto leg = lib::Vector3D::mulAdd(vert1.position, vert1.position);
		auto n = lib::Vector3D::subtract(vert1.position, vert2.position);
		n = lib::Vector3D::normalize(n);//正規化2
		return lib::Vector3D::scale(n, constant * (length - leg));
	}
	DirectX::XMFLOAT3 ClothSimulatorOld::spring(
		int id,
		int step,
		int vertex_num,
		Vertex vert1,
		Vertex vert2,
		float length,
		float shrink,
		float stretch
	) {
		/*if (vert2.color.x == 1.f &&
			vert2.color.y == 0.f &&
			vert2.color.z == 0.f)return vert1.position;*/
		auto spr_const = m_Phys->param.spring.constant * m_Phys->param.mass;
		auto d = lib::Vector3D::distance(vert1.position, vert2.position);
		float f = (d - length) * spr_const;
		f >= 0 ? f *= shrink : f *= stretch;
		auto cons_dtX = lib::Vector3D::subtract(vert2.position, vert1.position);
		cons_dtX = lib::Vector3D::normalize(cons_dtX);
		cons_dtX = lib::Vector3D::scale(cons_dtX, f);
		cons_dtX = lib::Vector3D::scale(cons_dtX, step * step * 0.5 / m_Phys->param.mass);
		//return lib::Vector3D::scale(cons_dtX, weigth[vertex_num] / (weigth[vertex_num] + weigth[id]));
		auto dx_p1 = lib::Vector3D::scale(cons_dtX, weigth[vertex_num] / (weigth[vertex_num] + weigth[id]));
		return lib::Vector3D::add(vert1.position, dx_p1);
	}
	std::vector<DirectX::XMFLOAT3> inline ClothSimulatorOld::createVirtualPointData(
		int vertexNum,
		std::vector<Vertex> vertex,
		std::vector<UINT> index,
		std::vector<int>* per_index
	) {
		std::vector<DirectX::XMFLOAT3> virVertex;
		virVertex.resize(5);
		virVertex[0] = vertex[vertexNum].position;
		if (between(per_index[0][vertexNum], 0, vertex.size())) {
			virVertex[1] = vertex[per_index[0][vertexNum]].position;
		}
		if (between(per_index[1][vertexNum], 0, vertex.size())) {
			virVertex[2] = vertex[per_index[1][vertexNum]].position;
		}
		if (between(per_index[2][vertexNum], 0, vertex.size())) {
			virVertex[3] = vertex[per_index[2][vertexNum]].position;
		}
		if (between(per_index[3][vertexNum], 0, vertex.size())) {
			virVertex[4] = vertex[per_index[3][vertexNum]].position;
		}
		return virVertex;
	}
	bool inline ClothSimulatorOld::between(float param, float deltaP1, float deltaP2) {
		if (deltaP1 <= param && param < deltaP2)return true;
		return false;
	}
	ClothSimulatorOld::~ClothSimulatorOld() {
	}
}