#include "ClothSimulator.h"
#include <iostream>
#include <fstream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "MassSpringModel/MassSpringModel.h"
#include "SpringForceCalculator/SpringForceCalculator.h"
#include "ClothShader/ClothShader.h"
namespace phy {
	std::vector<std::vector<ClothData>> ClothSimulator::m_ClothData;
	int ClothSimulator::m_time = 0;
	ClothSimulator::ClothSimulator() {

	}
	void ClothSimulator::createMassModel(
		std::vector<MassModel>& mass_model,
		std::vector<lib::ModelVertex> vertex,
		std::vector<UINT> index,
		std::vector<std::vector<int>> index_group
	) {
		mass_model.resize(vertex.size());
		MassSpringModel ms(MODEL_FILE::PMX, vertex, index, index_group);
		for (int ite = 0; ite < vertex.size(); ite++) {
			auto m3x3_4 = ms.create(ite);
			mass_model[ite].id0 = m3x3_4[0];
			mass_model[ite].id1 = m3x3_4[1];
			mass_model[ite].id2 = m3x3_4[2];
			mass_model[ite].id3 = m3x3_4[3];
			mass_model[ite].id4 = m3x3_4[4];
			mass_model[ite].id5 = m3x3_4[5];
			mass_model[ite].id6 = m3x3_4[6];
			mass_model[ite].id7 = m3x3_4[7];
			mass_model[ite].id8 = m3x3_4[8];
			mass_model[ite].id9 = m3x3_4[9];
			mass_model[ite].id10 = m3x3_4[10];
			mass_model[ite].id11 = m3x3_4[11];
		}
	}
	void ClothSimulator::createClothData(
		int handle,
		lib::ModelData& model,
		std::vector<std::vector<int>>& mass_spring_id,
		std::vector<SpringData>& spring_data
	) {
		int new_handle = handle + 1;
		if (m_ClothData.size() <= handle)m_ClothData.resize(new_handle);
		m_ClothData[handle].resize(model.vertex.size());
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			m_ClothData[handle][ite].pos = model.vertex[ite].position;
			m_ClothData[handle][ite].pre_pos = model.pre_vert[ite].position;
			m_ClothData[handle][ite].color = model.pre_vert[ite].color;
			m_ClothData[handle][ite].id0 = mass_spring_id[ite][0];
			m_ClothData[handle][ite].id1 = mass_spring_id[ite][1];
			m_ClothData[handle][ite].id2 = mass_spring_id[ite][2];
			m_ClothData[handle][ite].id3 = mass_spring_id[ite][3];
			m_ClothData[handle][ite].id4 = mass_spring_id[ite][4];
			m_ClothData[handle][ite].id5 = mass_spring_id[ite][5];
			m_ClothData[handle][ite].id6 = mass_spring_id[ite][6];
			m_ClothData[handle][ite].id7 = mass_spring_id[ite][7];
			m_ClothData[handle][ite].id8 = mass_spring_id[ite][8];
			m_ClothData[handle][ite].id9 = mass_spring_id[ite][9];
			m_ClothData[handle][ite].id10 = mass_spring_id[ite][10];
			m_ClothData[handle][ite].id11 = mass_spring_id[ite][11];
		}
	}
	void ClothSimulator::resetPower(std::vector<SpringData>& spring_data) {
		m_time = 0;
		for (int ite = 0; ite < spring_data.size(); ite++) {
			spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			spring_data[ite].velocity = DirectX::XMFLOAT3(0, 0, 0);
		}
	}
	void ClothSimulator::setupForce(
		int size,
		std::vector<SpringData>& spring_data
	) {
		spring_data.resize(size);
		for (int ite = 0; ite < size; ite++) {
			spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			spring_data[ite].velocity = DirectX::XMFLOAT3(0, 0, 0);
		}
	}
	void ClothSimulator::update(
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		const int step = 1;//15;
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
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring_data,
		std::shared_ptr<lib::DirectX12Manager>& dx_12
	) {
		const int step = 5;
		ClothShader cloth(dx_12);
		cloth.execution(model_id, step, m_time, model, mass_model, spring_data);
		m_time++;
	}
	std::vector<std::vector<int>>
	ClothSimulator::getPreIndexID() {
		return m_Pre_IndexID;
	}
	ClothSimulator::~ClothSimulator() {

	}
}