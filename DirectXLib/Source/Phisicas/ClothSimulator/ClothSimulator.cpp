#include "ClothSimulator.h"
#include <iostream>
#include <fstream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "MassSpringModel/MassSpringModel.h"
#include "SpringForceCalculator/SpringForceCalculator.h"
#include "ClothSpringShader/ClothSpringShader.h"
namespace phy {
	int ClothSimulator::m_time = 0;
	ClothSimulator::ClothSimulator() {

	}
	ClothSimulator::ClothSimulator(std::vector<lib::ModelParam> vertex, std::vector<UINT> index, std::vector<std::vector<int>> index_group) {
		m_Pre_IndexID.resize(vertex.size());
		MassSpringModel ms(MODEL_FILE::PMX,vertex, index, index_group);
		for (int ite = 0; ite < vertex.size(); ite++) {
			if (vertex[ite].color.x == 1.f && 
				vertex[ite].color.y == 0.f && 
				vertex[ite].color.z == 0.f
				)continue;
			m_Pre_IndexID[ite] = ms.create(ite);
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
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		const int step = 1;
		SpringForceCalculator force(model.pre_vert);
		//モデル全頂点の力と速度データ受け取り
		if (spring_data.size() > 0) force.setSpringForceData(spring_data);
		//重力を加える
		force.gravity(m_time, model.vertex, mass_spring_id);
		//ステップ数だけバネの計算をする
		for (int i = 0; i < step; i++) {
			force.restriction(m_time, model.vertex, mass_spring_id);
		}
		//force.collision(vertex);
		spring_data = force.getSpringForceData();
		m_time++;
	}
	void ClothSimulator::execution(
		int model_id,
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id,
		std::shared_ptr<lib::DirectX12Manager>& dx_12
	) {
		const int step = 1;
		ClothSpringShader cloth_shader(model_id, dx_12);
		cloth_shader.create(model, spring_data, mass_spring_id);
		SpringForceCalculator force(model.pre_vert);
		//モデル全頂点の力と速度データ受け取り
		if (spring_data.size() > 0) force.setSpringForceData(spring_data);
		//重力を加える
		force.gravity(m_time, model.vertex, mass_spring_id);
		//ステップ数だけバネの計算をする
		for (int i = 0; i < step; i++) {
			cloth_shader.execution();
		}
		//force.collision(vertex);
		spring_data = cloth_shader.getSpring();
		m_time++;
	}
	std::vector<std::vector<int>>
	ClothSimulator::getPreIndexID() {
		return m_Pre_IndexID;
	}
	ClothSimulator::~ClothSimulator() {

	}
}