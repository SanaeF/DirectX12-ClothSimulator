#include "ClothCollider.h"
#include "../../../../VectorMath/VectorMath.h"
#include <cassert>
namespace phy {
	std::vector<std::vector<int>> ClothCollider::m_Hit_space;
	ClothCollider::ClothCollider(int split_num, int vert_size) :
		m_Split_num(split_num),
		m_Vert_size(vert_size),
		m_Hit_dist(0.f),
		m_Now_dist(0.f),
		m_High_dist(0.f, 0.f, 0.f),
		m_Low_dist(0.f, 0.f, 0.f),
		m_High_pos(0.f, 0.f, 0.f),
		m_Low_pos(0.f, 0.f, 0.f),
		m_Position_zero(0.f, 0.f, 0.f)
	{
		m_Space_vert.resize(vert_size);
	}
	//モデルサイズに合わせて、あたり判定空間を生成する
	void ClothCollider::createSpaceBox(lib::ModelVertex p) {
		//四角形p1を求める
		if (m_High_dist.x < p.position.x)m_High_dist.x = p.position.x;
		if (m_High_dist.y < p.position.y)m_High_dist.y = p.position.y;
		if (m_High_dist.z < p.position.z)m_High_dist.z = p.position.z;
		//四角形p2を求める
		if (m_Low_dist.x > p.position.x)m_Low_dist.x = p.position.x;
		if (m_Low_dist.y > p.position.y)m_Low_dist.y = p.position.y;
		if (m_Low_dist.z > p.position.z)m_Low_dist.z = p.position.z;
		m_High_pos = DirectX::XMFLOAT3(m_High_dist.x, m_High_dist.y, m_High_dist.z);
		m_Low_pos = DirectX::XMFLOAT3(m_Low_dist.x, m_Low_dist.y, m_Low_dist.z);
		if (m_High_pos.x == m_Low_pos.x && m_High_pos.y == m_Low_pos.y && m_High_pos.z == m_Low_pos.z)return;
		m_Position_zero = lib::VectorMath::slplitPoint(m_High_pos, m_Low_pos);
	}
	void ClothCollider::spaceInput(int id, lib::ModelVertex p) {
		int box = 1;
		if (m_High_pos.x != 0)box *= m_Split_num;
		if (m_High_pos.y != 0)box *= m_Split_num;
		if (m_High_pos.z != 0)box *= m_Split_num;
		m_Box = box;
		box = box * 8;
		if (m_Input_num.size() == 0)m_Input_num.resize(box + 1);
		if (m_Hit_space.size() == 0) {
			m_Hit_space.resize(box + 1);
			for (int ite = 0; ite < m_Hit_space.size(); ite++) {
				m_Hit_space[ite].resize(m_Vert_size);
			}
		}
		if (m_Hit_space.size() == 1)return;
		int space_id = searchX(id, m_Split_num, p.position);
		//頂点IDごとに空間IDを指定
		m_Space_vert[id] = space_id;
		m_Hit_space[space_id][m_Input_num[space_id]] = id;
		//空間IDごとの頂点の個数
		m_Input_num[space_id]++;
	}
	void ClothCollider::repulsion(
		int vert_id,
		SpringData& spring_data,
		std::vector<lib::ModelVertex>& vert,
		std::vector<lib::ModelVertex>& pre_vert
	) {
		if (m_Hit_space.size() == 1)return;
		auto id = m_Space_vert[vert_id];
		//空間IDが一致する頂点を比較する
		for (int ite = 0; ite < m_Input_num[id]; ite++) {
			//continue;
			auto id2 = m_Hit_space[id][ite];
			m_Hit_dist = lib::VectorMath::distance(pre_vert[vert_id].position, pre_vert[id2].position);
			if (isHitCircle(vert[vert_id].position, vert[id2].position)) {
				auto f = solver(vert[vert_id].position, vert[id2].position);
				spring_data.force = lib::VectorMath::add(spring_data.force, f);
			}
		}
	}
	inline int ClothCollider::searchX(
		int id, int split_num, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.x == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より右
			auto size = m_High_pos.x / split_num;
			if (p.x == 0 ||
				p.x == m_High_pos.x ||
				between(p.x, (size * ite) + m_Position_zero.x, (size * (ite + 1)) + m_Position_zero.x)
				) {
				m_Add_box_id = 0;
				return searchY(id, m_Split_num, ite, p);
			}
			size = m_Low_pos.x / split_num;
			if (p.x == m_Low_pos.x ||
				between(p.x, (size * (split_num - ite)) + m_Position_zero.x, (size * ite) + m_Position_zero.x)
				) {
				m_Add_box_id = m_Box * 4;
				return searchY(id, m_Split_num, ite, p);
			}
		}
		//assert(0 && "searchXで空間分割に失敗しました。");
		return 0;
	}
	inline int ClothCollider::searchY(
		int id, int split_num, int ite_x, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.y == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より上
			auto size = m_High_pos.y / split_num;
			if (p.y == 0 ||
				p.y == m_High_pos.y ||
				between(p.y, (size * ite) + m_Position_zero.y, (size * (ite + 1)) + m_Position_zero.y)
				) {
				return searchZ(id, m_Split_num, ite_x, ite, p);
			}
			size = m_Low_pos.y / split_num;
			if (p.y == m_Low_pos.y ||
				between(p.y, (size * (split_num - ite)) + m_Position_zero.y, (size * ite) + m_Position_zero.y)
				) {
				m_Add_box_id += m_Box * 2;
				return searchZ(id, m_Split_num, ite_x, ite, p);
			}
		}
		//assert(0 && "searchYで空間分割に失敗しました。");
		return 0;
	}
	inline int ClothCollider::searchZ(
		int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.z == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より奥
			auto size = m_High_pos.z / split_num;
			if (p.z == 0 ||
				p.z == m_High_pos.z ||
				between(p.z, (size * ite) + m_Position_zero.z, (size * (ite + 1)) + m_Position_zero.z)
				) {
				return m_Add_box_id + (ite_x + ite_y + ite);
			}
			size = m_Low_pos.z / split_num;
			if (p.z == m_Low_pos.z ||
				between(p.z, (size * (split_num - ite)) + m_Position_zero.z, (size * ite) + m_Position_zero.z)
				) {
				m_Add_box_id += m_Box;
				return m_Add_box_id + (ite_x + ite_y + ite);
			}
		}
		//assert(0 && "searchZで空間分割に失敗しました。");
		return 0;
	}
	inline bool ClothCollider::isHitCircle(
		DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2
	) {
		m_Now_dist = lib::VectorMath::distance(p1, p2);
		if (m_Now_dist < m_Hit_dist)return true;
		return false;
	}
	inline DirectX::XMFLOAT3
		ClothCollider::solver(
			DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2
		) {

		auto n = lib::VectorMath::subtract(p1, p2);
		float f = m_Hit_dist - m_Now_dist;
		return lib::VectorMath::scale(n, f);
	}
	inline bool ClothCollider::between(float p1, float a, float b) {
		if (a <= p1 && p1 < b)return true;
		return false;
	}
}