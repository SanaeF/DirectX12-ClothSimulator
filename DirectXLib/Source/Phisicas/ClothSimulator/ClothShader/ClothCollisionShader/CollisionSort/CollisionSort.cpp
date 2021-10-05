#include "CollisionSort.h"
#include "../../../../../VectorMath/VectorMath.h"
namespace phy {
	CollisionSort::CollisionSort(int vert_size, DirectX::XMFLOAT3& max, DirectX::XMFLOAT3& min) :
		m_High_pos(max),
		m_Low_pos(min)
	{
		m_Split_num = HIT_SPLIT::SINGLE;
		m_Position_zero = lib::VectorMath::slplitPoint(m_High_pos, m_Low_pos);
	}
	int CollisionSort::spaceInput(int id, DirectX::XMFLOAT3& p) {
		int box = 1;
		if (m_High_pos.x != 0)box *= m_Split_num;
		if (m_High_pos.y != 0)box *= m_Split_num;
		if (m_High_pos.z != 0)box *= m_Split_num;
		m_Box = box;
		box = box * 8;
		//if (m_Hit_space.size() == 1)return;
		int space_id = searchX(id, m_Split_num, p);
		return space_id;
	}
	inline int CollisionSort::searchX(
		int id, int split_num, DirectX::XMFLOAT3& p
	) {
		if (m_High_pos.x == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//Ž²‚æ‚è‰E
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
		//assert(0 && "searchX‚Å‹óŠÔ•ªŠ„‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return 0;
	}
	inline int CollisionSort::searchY(
		int id, int split_num, int ite_x, DirectX::XMFLOAT3& p
	) {
		if (m_High_pos.y == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//Ž²‚æ‚èã
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
		//assert(0 && "searchY‚Å‹óŠÔ•ªŠ„‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return 0;
	}
	inline int CollisionSort::searchZ(
		int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3& p
	) {
		if (m_High_pos.z == 0)split_num = 1;
		for (int ite = 0; ite < split_num; ite++) {//Ž²‚æ‚è‰œ
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
		//assert(0 && "searchZ‚Å‹óŠÔ•ªŠ„‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return 0;
	}
	inline bool CollisionSort::between(float p1, float a, float b) {
		if (a <= p1 && p1 < b)return true;
		return false;
	}
}