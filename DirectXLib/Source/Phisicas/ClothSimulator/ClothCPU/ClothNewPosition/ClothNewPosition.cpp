#include "ClothNewPosition.h"
#include "../../DirectXLib/Source/VectorMath/VectorMath.h"
namespace phy {
	ClothNewPosition::ClothNewPosition(
		ClothForce& world_f,
		std::vector<lib::ModelVertex>& vert,
		std::vector<SpringData>& spring,
		std::vector<PolygonModel>& polygon_model
	) {
		m_World_f = world_f;
		m_Vert = vert;
		m_Spring = spring;
		m_Polygon_model = polygon_model;
		m_Frame.max_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_Frame.min_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}
	void ClothNewPosition::execution(
		std::vector<lib::ModelVertex>& vert,
		std::vector<SpringData>& spring,
		bool no_add_vel
	) {
		m_Is_no_add_vel = no_add_vel;
		for (int ite = 0; ite < m_Vert.size(); ite++) {
			executionPoint(ite);
		}
		vert = m_Vert;
		spring = m_Spring;
	}
	void ClothNewPosition::PositionSort(int id) {
		if (m_Frame.max_pos.x < m_Vert[id].position.x)m_Frame.max_pos.x = m_Vert[id].position.x;
		if (m_Frame.max_pos.y < m_Vert[id].position.y)m_Frame.max_pos.y = m_Vert[id].position.y;
		if (m_Frame.max_pos.z < m_Vert[id].position.z)m_Frame.max_pos.z = m_Vert[id].position.z;
		if (m_Frame.min_pos.x > m_Vert[id].position.x)m_Frame.min_pos.x = m_Vert[id].position.x;
		if (m_Frame.min_pos.y > m_Vert[id].position.y)m_Frame.min_pos.y = m_Vert[id].position.y;
		if (m_Frame.min_pos.z > m_Vert[id].position.x)m_Frame.min_pos.z = m_Vert[id].position.z;
	}
	void ClothNewPosition::executionPoint(int id) {
		if (isFixed(m_Vert[id]))return;
		float dt = m_World_f.dt / 100;
		DirectX::XMFLOAT3 v = lib::VectorMath::scale(m_Spring[id].force, m_Spring[id].mass / (m_Spring[id].mass + m_Spring[id].mass));
		v = lib::VectorMath::scale(v, dt);
		m_Spring[id].velocity = lib::VectorMath::add(m_Spring[id].velocity, v);
		v = lib::VectorMath::scale(m_Spring[id].velocity, dt);
		m_Vert[id].position = lib::VectorMath::add(m_Vert[id].position, v);
		//力をゼロにする
		m_Spring[id].force = DirectX::XMFLOAT3(0, 0, 0);

		//頂点の法線
		int size = m_Polygon_model[id].polygon_num;
		DirectX::XMFLOAT3 normal(0, 0, 0);
		for (int ite = 0; ite < size; ite++) {
			//有効な三角形か確認
			DirectX::XMINT3 polygon = m_Polygon_model[id].id[ite];
			normal = lib::VectorMath::add(
				normal, 
				lib::VectorMath::createNormal(
					m_Vert[polygon.x].position,
					m_Vert[polygon.y].position,
					m_Vert[polygon.z].position
				)
			);
		}
		//normal = scale(normal, 1 / size);
		normal = lib::VectorMath::normalize(normal);
		m_Vert[id].normal = DirectX::XMFLOAT4(normal.x, normal.y, normal.z, m_Vert[id].normal.w);
		PositionSort(id);
	}
	inline bool ClothNewPosition::isFixed(lib::ModelVertex vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
	ClothNewPosition::PositionFrame 
		ClothNewPosition::getFrame() {
		return m_Frame;
	}
}