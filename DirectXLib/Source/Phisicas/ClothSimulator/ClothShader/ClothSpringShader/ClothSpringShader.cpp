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
			shaderHandler[m_Model_id].sim_param.resize(1);
		}
	}
	void ClothSpringShader::create(
		ClothForce& world_f,
		lib::ModelData& model,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring,
		std::vector<lib::ModelVertex>& pre_vert
	) {
		if (65535 < model.vertex.size()) {
			assert(0 && "<このモデルは上限を超えています!>DirectX12の使用により、クロスシミュレーターに使用できる頂点数は65535個が限界です。");
		}
		auto& is_input = shaderHandler[m_Model_id].is_created;
		if (!is_input) {
			shaderHandler[m_Model_id].sim_param[0].vert_max = model.vertex.size();
			shaderHandler[m_Model_id].sim_param[0].tension = world_f.tension;
			shaderHandler[m_Model_id].sim_param[0].compress = world_f.compress;
			shaderHandler[m_Model_id].sim_param[0].share = world_f.share;
			shaderHandler[m_Model_id].sim_param[0].bend = world_f.bend;
			shaderHandler[m_Model_id].sim_param[0].dt = world_f.dt;
			shaderHandler[m_Model_id].sim_param[0].k = world_f.k;

			int size = sqrt(model.vertex.size());
			int size_out = model.vertex.size() - (size * size);
			auto& thread = shaderHandler[m_Model_id].thread;
			thread.x = size + size_out;
			thread.y = size;
			thread.z = 1;

			m_Shader.reset(
				new lib::ComputeShader(
					"./DirectXLib/Shader/Shader3D/Cloth/ClothSpring.hlsl",
					"ClothSpring", 6, shaderHandler[m_Model_id].compute_handle, m_Dx12)
			);
			//取得用データ
			m_Shader->inputBufferSize(0, spring.size(), sizeof(SpringData));//シミュレートして変わったバネ情報
			//転送用のモデルデータ
			m_Shader->inputBufferSize(1, shaderHandler[m_Model_id].sim_param.size(), sizeof(SimulateParam));//シミュレート情報
			m_Shader->inputBufferSize(2, mass_model.size(), sizeof(MassModel));//質点ID
			m_Shader->inputBufferSize(3, pre_vert.size(), sizeof(lib::ModelVertex));//固定用のモデル情報
			m_Shader->inputBufferSize(4, model.vertex.size(), sizeof(lib::ModelVertex));//描画で使ったモデル情報
			m_Shader->inputBufferSize(5, spring.size(), sizeof(SpringData));//バネモデル情報
			m_Shader->createUnorderdAccessView();

			m_Shader->mapOutput(0);
		}
		else m_Shader.reset(new lib::ComputeShader(shaderHandler[m_Model_id].compute_handle, m_Dx12));
		if (!is_input) {
			m_Shader->mapInput(1, shaderHandler[m_Model_id].sim_param.data());
			m_Shader->mapInput(2, mass_model.data());
			m_Shader->mapInput(3, pre_vert.data());
		}
		m_Shader->mapInput(4, model.vertex.data());
		m_Shader->mapInput(5, spring.data());
		is_input = true;
	}

	void ClothSpringShader::execution(std::vector<SpringData>& spring) {
		m_Shader->execution(
			shaderHandler[m_Model_id].thread.x,
			shaderHandler[m_Model_id].thread.y,
			shaderHandler[m_Model_id].thread.z,
			shaderHandler[m_Model_id].compute_handle
		);
		dataAssign(spring);
	}
	void ClothSpringShader::dataAssign(std::vector<SpringData>& spring) {
		spring.assign(
			(SpringData*)m_Shader->getData(0),
			(SpringData*)m_Shader->getData(0) + spring.size());
	}
	ClothSpringShader::~ClothSpringShader() {
	}
}