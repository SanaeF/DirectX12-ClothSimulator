#pragma once
#include <vector>
#include<d3dx12.h>
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
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
		DirectX::XMFLOAT3 m_Position_zero;
		DirectX::XMFLOAT3 m_High_dist;
		DirectX::XMFLOAT3 m_Low_dist;
		DirectX::XMFLOAT3 m_High_pos;//モデル空間p1
		DirectX::XMFLOAT3 m_Low_pos;//モデル空間p2
	public:
		ClothCollider(int split_num, int vert_size);
		//モデルサイズに合わせて、あたり判定空間を生成する
		void createSpaceBox(lib::ModelParam p);
		void spaceInput(int id, lib::ModelParam p);
		void repulsion(
			int vert_id,
			SpringData& spring_data,
			std::vector<lib::ModelParam>& vert,
			std::vector<lib::ModelParam>& pre_vert
		);
	private:
		bool isHitCircle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		DirectX::XMFLOAT3 solver(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		int searchX(int id, int split_num, DirectX::XMFLOAT3 p);
		int searchY(int id, int split_num, int ite_x, DirectX::XMFLOAT3 p);
		int searchZ(int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3 p);
		//p1がa～bの間に含まれているかどうか
		bool between(float p1, float a, float b);
	};
}