#include "ClothSimulator.h"
#include <iostream>
#include <fstream>
#include "MassSpringModel/MassSpringModel.h"
#include "SpringForceCalculator/SpringForceCalculator.h"
namespace phy {
	int ClothSimulator::m_time = 0;
	ClothSimulator::ClothSimulator() {

	}
	ClothSimulator::ClothSimulator(std::vector<lib::ModelData> vertex, std::vector<UINT> index, std::vector<std::vector<int>> index_group) {
		m_Pre_IndexID.resize(vertex.size());
		MassSpringModel ms(vertex, index, index_group);
		for (int ite = 0; ite < vertex.size(); ite++) {
			auto id_list = ms.create(ite);
			for (int ite2 = 0; ite2 < 12; ite2++) {
				m_Pre_IndexID[ite] = id_list;
			}
		}
	}
	void ClothSimulator::resetPower(std::vector<SpringData>& spring_data) {
		m_time = 0;
		for (int ite = 0; ite < spring_data.size(); ite++) {
			spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			spring_data[ite].velocity = DirectX::XMFLOAT3(0, 0, 0);
		}
	}
	void ClothSimulator::update(
		std::vector<lib::ModelData>& vertex,
		std::vector<UINT> index,
		std::vector<lib::ModelData> pre_vertex,
		std::vector<std::vector<int>> pre_index_id,
		std::vector<SpringData>& spring_data
	) {
		const int step = 1;
		SpringForceCalculator force(pre_vertex);
		//モデル全頂点の力と速度データ受け取り
		if (spring_data.size() > 0) force.setSpringForceData(spring_data);
		//重力を加える
		force.gravity(m_time, vertex, pre_index_id);
		//座標更新
		//force.createNewPosition(vertex);
		//ステップ数だけバネの計算をする
		for (int i = 0; i < step; i++) {
			force.restriction(m_time, vertex, pre_index_id);
			//座標更新
			//force.createNewPosition(vertex);
		}
		//force.collision(vertex);
		spring_data = force.getSpringForceData();
		m_time++;
	}
	std::vector<std::vector<int>>
	ClothSimulator::getPreIndexID() {
		return m_Pre_IndexID;
	}
	ClothSimulator::~ClothSimulator() {

	}
}