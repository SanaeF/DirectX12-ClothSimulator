#include "ClothSpringShader.h"
#include <cassert>
#include <iostream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
#include "../../DirectXLib/Source/ShaderManager/ComputeShader/ComputeShader.h"

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace phy {
	std::vector<ClothSpringShader::CSSInfo> ClothSpringShader::shaderHandler;

	ClothSpringShader::ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12) :
		m_Model_id(model_id),
		m_Dx12(dx_12),
		m_Device(dx_12->device())
	{
		if (shaderHandler.size() <= m_Model_id)shaderHandler.resize(m_Model_id + 1);
		if (!shaderHandler[m_Model_id].is_created) {
			shaderHandler[m_Model_id].out_param.resize(1);
			shaderHandler[m_Model_id].out_param[0].is_simulated = false;
			shaderHandler[m_Model_id].sim_param.resize(1);
		}
	}
	void ClothSpringShader::create(
		lib::ModelData& model,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring
	) {
		if (65535 < model.vertex.size()) {
			assert(0 && "<このモデルは上限を超えています!>DirectX12の使用により、クロスシミュレーターに使用できる頂点数は65535個が限界です。");
		}
		auto& is_input = shaderHandler[m_Model_id].is_created;
		if (!is_input) {
			shaderHandler[m_Model_id].sim_param[0].vert_max = model.vertex.size();


			shaderHandler[m_Model_id].out_param[0].max_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
			shaderHandler[m_Model_id].out_param[0].min_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
			int size = sqrt(model.vertex.size());
			int size_out = model.vertex.size() - (size * size);
			auto& thread = shaderHandler[m_Model_id].thread;
			thread.x = size + size_out;
			thread.y = size;
			thread.z = 1;

			m_Shader.reset(
				new lib::ComputeShader(
					"./DirectXLib/Shader/Shader3D/Cloth/ClothSpring.hlsl",
					"ClothSpring", 9, shaderHandler[m_Model_id].compute_handle, m_Dx12)
			);
			//取得用データ
			m_Shader->inputBufferSize(0, model.vertex.size(), sizeof(lib::ModelVertex));//シミュレートして変わったモデル情報
			m_Shader->inputBufferSize(1, spring.size(), sizeof(SpringData));//シミュレートして変わったバネ情報
			m_Shader->inputBufferSize(2, shaderHandler[m_Model_id].out_param.size(), sizeof(ResultParam));//最大座標と最小座標の情報
			//転送用のモデルデータ
			m_Shader->inputBufferSize(3, shaderHandler[m_Model_id].sim_param.size(), sizeof(SimulateParam));//シミュレート情報
			m_Shader->inputBufferSize(4, mass_model.size(), sizeof(MassModel));//質点ID
			m_Shader->inputBufferSize(5, model.pre_vert.size(), sizeof(lib::ModelVertex));//固定用のモデル情報
			m_Shader->inputBufferSize(6, model.vertex.size(), sizeof(lib::ModelVertex));//描画で使ったモデル情報
			m_Shader->inputBufferSize(7, spring.size(), sizeof(SpringData));//バネモデル情報
			m_Shader->inputBufferSize(8, spring.size(), sizeof(SpringData));//バネモデル情報
			m_Shader->createUnorderdAccessView();

			m_Shader->mapOutput(0);
			m_Shader->mapOutput(1);
			m_Shader->mapOutput(2);
		}
		else m_Shader.reset(new lib::ComputeShader(shaderHandler[m_Model_id].compute_handle, m_Dx12));
		if (!is_input) {
			m_Shader->mapInput(3, shaderHandler[m_Model_id].sim_param.data());
			m_Shader->mapInput(4, mass_model.data());
			m_Shader->mapInput(5, model.pre_vert.data());
		}
		m_Shader->mapInput(6, model.vertex.data());
		m_Shader->mapInput(7, spring.data());
		m_Shader->mapInput(8, spring.data());
		is_input = true;
	}

	void ClothSpringShader::execution(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring) {
		m_Shader->execution(
			shaderHandler[m_Model_id].thread.x,
			shaderHandler[m_Model_id].thread.y,
			shaderHandler[m_Model_id].thread.z,
			m_Model_id
		);
		dataAssign(is_simulated, model, spring);
	}
	void ClothSpringShader::dataAssign(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring) {
		auto& hd = shaderHandler[m_Model_id];
		auto result = (lib::ModelVertex*)m_Shader->getData(0);
		if (is_simulated) {
			model.vertex.assign(
				(lib::ModelVertex*)m_Shader->getData(0),
				(lib::ModelVertex*)m_Shader->getData(0) + model.vertex.size());
			spring.assign(
				(SpringData*)m_Shader->getData(1),
				(SpringData*)m_Shader->getData(1) + spring.size());
		}
		hd.out_param.assign(
			(ResultParam*)m_Shader->getData(2),
			(ResultParam*)m_Shader->getData(2) + hd.out_param.size());
		if (hd.out_param[0].is_simulated)m_Is_simulated = true;
	}
	ClothSpringShader::ResultParam
		ClothSpringShader::getResultParam(int id) {
		return shaderHandler[id].out_param[0];
	}
	bool ClothSpringShader::isSimulated() {
		return m_Is_simulated;
	}
	ClothSpringShader::~ClothSpringShader() {
	}
}