#pragma once
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../../ClothData/SpringData.h"
namespace phy {
	class CollisionSort {
	private:
		int m_Add_box_id;
		int m_Split_num;
		int m_Box;
		DirectX::XMFLOAT3 m_Position_zero;
		DirectX::XMFLOAT3 m_High_pos;//モデル空間p1
		DirectX::XMFLOAT3 m_Low_pos;//モデル空間p2
	public:
		CollisionSort(int vert_size, DirectX::XMFLOAT3& max, DirectX::XMFLOAT3& min);
		//空間を割り当ててIDを返す
		int spaceInput(int id, DirectX::XMFLOAT3& p);
	private:
		int searchX(int id, int split_num, DirectX::XMFLOAT3& p);
		int searchY(int id, int split_num, int ite_x, DirectX::XMFLOAT3& p);
		int searchZ(int id, int split_num, int ite_x, int ite_y, DirectX::XMFLOAT3& p);
		//p1がa〜bの間に含まれているかどうか
		bool between(float p1, float a, float b);
	};
}