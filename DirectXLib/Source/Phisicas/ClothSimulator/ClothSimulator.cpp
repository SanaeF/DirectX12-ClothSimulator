#include "ClothSimulator.h"
#include <iostream>
#include <fstream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "MassSpringModel/MassSpringModel.h"
#include "VertexPolygonModel/VertexPolygonModel.h"
#include "SpringForceCalculator/SpringForceCalculator.h"
#include "ClothShader/ClothShader.h"
namespace phy {
	std::vector<lib::ClothHandleData> ClothSimulator::m_Cloth_handle_data;
	std::vector<std::vector<ClothData>> ClothSimulator::m_ClothData;
	int ClothSimulator::m_time = 0;
	ClothSimulator::ClothSimulator() {

	}
	void ClothSimulator::initialize(int handle, int step, lib::ModelData& model, ClothForce world_f) {
		if (m_Cloth_handle_data.size() <= handle)m_Cloth_handle_data.resize(handle + 1);
		int size = model.vertex.size();
		m_Cloth_handle_data[handle].spring_data.resize(size);
		//初期値の代入
		m_Cloth_handle_data[handle].pre_vert = model.vertex;
		m_Cloth_handle_data[handle].last_vert = model.vertex;
		//バネの物理パラメーター初期化
		for (int ite = 0; ite < size; ite++) {
			m_Cloth_handle_data[handle].spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
			m_Cloth_handle_data[handle].spring_data[ite].velocity = DirectX::XMFLOAT3(0, 0, 0);
		}
		//質点生成
		MassSpringModel mass_model(MODEL_FILE::PMX, model.vertex, model.index, model.index_group);
		m_Cloth_handle_data[handle].mass_model = mass_model.getData();
		/*m_Cloth_handle_data[handle].mass_model.resize(size);
		for (int ite = 0; ite < size; ite++) {
			auto m3x3_4 = ms.create(ite);
			m_Cloth_handle_data[handle].mass_model[ite].id0 = m3x3_4[0];
			m_Cloth_handle_data[handle].mass_model[ite].id1 = m3x3_4[1];
			m_Cloth_handle_data[handle].mass_model[ite].id2 = m3x3_4[2];
			m_Cloth_handle_data[handle].mass_model[ite].id3 = m3x3_4[3];
			m_Cloth_handle_data[handle].mass_model[ite].id4 = m3x3_4[4];
			m_Cloth_handle_data[handle].mass_model[ite].id5 = m3x3_4[5];
			m_Cloth_handle_data[handle].mass_model[ite].id6 = m3x3_4[6];
			m_Cloth_handle_data[handle].mass_model[ite].id7 = m3x3_4[7];
			m_Cloth_handle_data[handle].mass_model[ite].id8 = m3x3_4[8];
			m_Cloth_handle_data[handle].mass_model[ite].id9 = m3x3_4[9];
			m_Cloth_handle_data[handle].mass_model[ite].id10 = m3x3_4[10];
			m_Cloth_handle_data[handle].mass_model[ite].id11 = m3x3_4[11];
		}*/
		//頂点三角形のデータを生成
		VertexPolygonModel polygon(model);
		m_Cloth_handle_data[handle].polygon_model = polygon.getData();
		m_Cloth_handle_data[handle].world_f = world_f;
		m_Cloth_handle_data[handle].step = step;
		m_time = 0;
	}
	void ClothSimulator::worldForce(int handle, ClothForce world_f) {
		m_Cloth_handle_data[handle].world_f = world_f;
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
		//const int step = 1;//15;
		//SpringForceCalculator force(model.pre_vert);
		////モデル全頂点の力と速度データ受け取り
		//if (spring_data.size() > 0) force.setSpringForceData(spring_data);
		////重力を加える
		//force.gravity(m_time, model.vertex, mass_spring_id);
		////ステップ数だけバネの計算をする
		//for (int i = 0; i < step; i++) {
		//	force.restriction(m_time, model.vertex, mass_spring_id);
		//}
		////force.collision(vertex);
		//spring_data = force.getSpringForceData();
		//m_time++;
	}
	void ClothSimulator::resetModel(int handle, lib::ModelData& model) {
		ClothSimulator::resetPower(m_Cloth_handle_data[handle].spring_data);
		model.vertex = m_Cloth_handle_data[handle].pre_vert;
		model.bufferMap();
	}
	void ClothSimulator::execution(
		int handle,
		lib::ModelData& model,
		std::shared_ptr<lib::DirectX12Manager>& dx_12
	) {
		ClothShader cloth(dx_12);
		cloth.execution(
			handle, 
			m_Cloth_handle_data[handle].step,
			m_time, 
			m_Cloth_handle_data[handle].world_f,
			model,
			m_Cloth_handle_data[handle].pre_vert,
			m_Cloth_handle_data[handle].last_vert,
			m_Cloth_handle_data[handle].mass_model, 
			m_Cloth_handle_data[handle].spring_data,
			m_Cloth_handle_data[handle].polygon_model
		);
		model.bufferMap();
		m_time++;
	}
	ClothSimulator::~ClothSimulator() {

	}
}