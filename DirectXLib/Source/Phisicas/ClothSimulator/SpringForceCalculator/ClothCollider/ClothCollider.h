#pragma once
#include <vector>
#include<d3dx12.h>
#include <vector>
#include "../../../../Graphics/Graph3D/Vertex/Vertex.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class ClothCollider {
	private:
		static std::vector<std::vector<int>> m_Hit_space;
		std::vector<int> m_Space_vert;
		std::vector<int> m_Input_num;
		float m_Hit_dist;
		float m_Now_dist;
		int m_Vert_size;
		int m_Add_box_id;
		int m_Split_num;
		int m_Box;
		DirectX::XMFLOAT3 m_High_dist;
		DirectX::XMFLOAT3 m_Low_dist;
		DirectX::XMFLOAT3 m_High_pos;//���f�����p1
		DirectX::XMFLOAT3 m_Low_pos;//���f�����p2
	public:
		ClothCollider(int split_num, int vert_size);
		//���f���T�C�Y�ɍ��킹�āA�����蔻���Ԃ𐶐�����
		void createSpaceBox(lib::Vertex p);
		void spaceInput(int id, lib::Vertex p);
		void repulsion(int vert_id, std::vector<lib::Vertex>& vert, std::vector<lib::Vertex> pre_vert);
	private:
		bool isHitCircle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		void calcForce(DirectX::XMFLOAT3& p1, DirectX::XMFLOAT3 p2);
		int searchX(int id, int split_num, DirectX::XMFLOAT3 p);
		int searchY(int id, int split_num, int ite_x, DirectX::XMFLOAT3 p);
		int searchZ(int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3 p);
		//p1��a�`b�̊ԂɊ܂܂�Ă��邩�ǂ���
		bool between(float p1, float a, float b);
	};
}