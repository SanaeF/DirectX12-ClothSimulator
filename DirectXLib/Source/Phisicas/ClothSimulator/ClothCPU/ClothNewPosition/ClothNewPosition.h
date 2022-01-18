#pragma once
#include <vector>
#include "../../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../ClothData/SpringData.h"
namespace phy {
	class ClothNewPosition {
	private:
		struct PositionFrame {
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		bool m_Is_no_add_vel;
		PositionFrame m_Frame;
		ClothForce m_World_f;
		std::vector <lib::ModelVertex> m_Vert;
		std::vector<SpringData> m_Spring;
		std::vector<PolygonModel> m_Polygon_model;
	public:
		ClothNewPosition(
			ClothForce& world_f,
			std::vector<lib::ModelVertex>& vert,
			std::vector<SpringData>& spring,
			std::vector<PolygonModel>& polygon_model
		);
		void execution(
			std::vector<lib::ModelVertex>& vert,
			std::vector<SpringData>& spring,
			bool no_add_vel
		);
		PositionFrame getFrame();
	private:
		void PositionSort(int id);
		void executionPoint(int id);
		bool isFixed(lib::ModelVertex vert);
	};
}