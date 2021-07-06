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
		m_Low_pos(0.f, 0.f, 0.f)
	{
		m_Space_vert.resize(vert_size);
	}
	//モデルサイズに合わせて、あたり判定空間を生成する
	void ClothCollider::createSpaceBox(lib::ModelData p) {
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
	}
	void ClothCollider::spaceInput(int id, lib::ModelData p) {
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
		std::vector<lib::ModelData>& vert,
		std::vector<lib::ModelData> pre_vert
	) {
		if (m_Hit_space.size() == 1)return;
		auto id = m_Space_vert[vert_id];
		//空間IDが一致する頂点を比較する
		for (int ite = 0; ite < m_Input_num[id]; ite++) {
			//continue;
			auto id2 = m_Hit_space[id][ite];
			m_Hit_dist = lib::VectorMath::distance(pre_vert[vert_id].position, pre_vert[id2].position) * 5 / 6;
			if (isHitCircle(vert[vert_id].position, vert[id2].position)) {
				calcForce(vert[vert_id].position, vert[id2].position);
			}
		}
	}
	inline int ClothCollider::searchX(
		int id, int split_num, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.x == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より右
			auto size = m_High_pos.x / split_num;
			if (p.x == 0 || p.x == m_High_pos.x || between(p.x, size * ite, size * (ite + 1))) {
				m_Add_box_id = 0;
				return searchY(id, m_Split_num, ite, p);
			}
			size = m_Low_pos.x / split_num;
			if (p.x == m_Low_pos.x || between(p.x, size * (split_num - ite), size * ite)) {
				m_Add_box_id = m_Box * 4;
				return searchY(id, m_Split_num, ite, p);
			}
		}
		assert("searchXで空間分割に失敗しました。");
		return 0;
	}
	inline int ClothCollider::searchY(
		int id, int split_num, int ite_x, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.y == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より上
			auto size = m_High_pos.y / split_num;
			if (p.y == 0 || p.y == m_High_pos.y || between(p.y, size * ite, size * (ite + 1))) {
				return searchZ(id, m_Split_num, ite_x, ite, p);
			}
			size = m_Low_pos.y / split_num;
			if (p.y == m_Low_pos.y || between(p.y, size * (split_num - ite), size * ite)) {
				m_Add_box_id += m_Box * 2;
				return searchZ(id, m_Split_num, ite_x, ite, p);
			}
		}
		assert("searchYで空間分割に失敗しました。");
		return 0;
	}
	inline int ClothCollider::searchZ(
		int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3 p
	) {
		if (m_High_pos.z == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//軸より奥
			auto size = m_High_pos.z / split_num;
			if (p.z == 0 || p.z == m_High_pos.z || between(p.z, size * ite, size * (ite + 1))) {
				return m_Add_box_id + (ite_x + ite_y + ite);
			}
			size = m_Low_pos.z / split_num;
			if (p.z == m_Low_pos.z || between(p.z, size * (split_num - ite), size * ite)) {
				m_Add_box_id += m_Box;
				return m_Add_box_id + (ite_x + ite_y + ite);
			}
		}
		assert("searchZで空間分割に失敗しました。");
		return 0;
	}
	inline bool ClothCollider::isHitCircle(
		DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2
	) {
		m_Now_dist = lib::VectorMath::distance(p1, p2);
		m_Now_dist = lib::VectorMath::distance(p1, DirectX::XMFLOAT3(0, -3, 0));
		m_Hit_dist = 0.75;
		if (m_Now_dist < m_Hit_dist)return true;
		return false;
	}
	inline void ClothCollider::calcForce(
		DirectX::XMFLOAT3& p1, DirectX::XMFLOAT3 p2
	) {
		p2 = DirectX::XMFLOAT3(0,-3,0);
		auto v = lib::VectorMath::subtract(p1, p2);
		auto d = lib::VectorMath::mulAdd(v, v);
		v = lib::VectorMath::scale(v, 0.75 / d);
		auto result = lib::VectorMath::add(p2, v);
		if (lib::VectorMath::distance(p1, result) < d)p1 = result;
	}
	inline bool ClothCollider::between(float p1, float a, float b) {
		if (a <= p1 && p1 < b)return true;
		return false;
	}
}