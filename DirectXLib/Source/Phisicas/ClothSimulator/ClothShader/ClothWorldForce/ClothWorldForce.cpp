#include "ClothWorldForce.h"
#include <cassert>
#include <iostream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
#include "../../DirectXLib/Source/ShaderManager/ComputeShader/ComputeShader.h"

namespace phy {
	std::vector<ClothWorldForce::CSSInfo> ClothWorldForce::m_ShaderHandler;
	ClothWorldForce::ClothWorldForce(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12):
		m_Dx12(dx_12) ,
		m_Model_id(model_id)
	{
		if (m_ShaderHandler.size() <= m_Model_id)m_ShaderHandler.resize(m_Model_id + 1);
		if (!m_ShaderHandler[m_Model_id].is_created) {
			m_ShaderHandler[m_Model_id].sim_param.resize(1);
		}
	}
	void ClothWorldForce::execution(
		int time,
		ClothForce& world_f,
		std::vector<lib::ModelVertex>& vert,
		std::vector<SpringData>& spring
	) {
		if (65535 < vert.size()) {
			assert(0 && "<このモデルは上限を超えています!>DirectX12の使用により、クロスシミュレーターに使用できる頂点数は65535個が限界です。");
		}
		auto& is_input = m_ShaderHandler[m_Model_id].is_created;
		m_ShaderHandler[m_Model_id].sim_param[0].wind = world_f.wind;
		if (!is_input) {
			m_ShaderHandler[m_Model_id].sim_param.resize(1);
			m_ShaderHandler[m_Model_id].frame.resize(1);
			m_ShaderHandler[m_Model_id].sim_param[0].vert_max = vert.size();
			m_ShaderHandler[m_Model_id].sim_param[0].time = time;
			m_ShaderHandler[m_Model_id].sim_param[0].tension = world_f.tension;
			m_ShaderHandler[m_Model_id].sim_param[0].compress = world_f.compress;
			m_ShaderHandler[m_Model_id].sim_param[0].share = world_f.share;
			m_ShaderHandler[m_Model_id].sim_param[0].bend = world_f.bend;
			m_ShaderHandler[m_Model_id].sim_param[0].dt = world_f.dt;
			m_ShaderHandler[m_Model_id].sim_param[0].k = world_f.k;
			m_ShaderHandler[m_Model_id].sim_param[0].damping = world_f.damping;
			m_ShaderHandler[m_Model_id].sim_param[0].gravity = world_f.gravity;
			m_ShaderHandler[m_Model_id].sim_param[0].grid_mass = world_f.grid_mass;
			int size = sqrt(vert.size());
			int size_out = vert.size() - (size * size);
			auto& thread = m_ShaderHandler[m_Model_id].thread;
			thread.x = size + size_out;
			thread.y = size;
			thread.z = 1;
			m_Shader.reset(
				new lib::ComputeShader(
					"./DirectXLib/Shader/Shader3D/Cloth/ClothWorldForce.hlsl",
					"ClothWorldForce", 6, m_ShaderHandler[m_Model_id].compute_handle, m_Dx12)
			);
			//取得用データ
			m_Shader->inputBufferSize(0, vert.size(), sizeof(lib::ModelVertex));//シミュレートして変わったモデル情報
			m_Shader->inputBufferSize(1, spring.size(), sizeof(SpringData));//シミュレートして変わったバネ情報
			m_Shader->inputBufferSize(2, m_ShaderHandler[m_Model_id].frame.size(), sizeof(PositionFrame));
			//転送用のモデルデータ
			m_Shader->inputBufferSize(3, m_ShaderHandler[m_Model_id].sim_param.size(), sizeof(SimulateParam));//シミュレート情報
			m_Shader->inputBufferSize(4, vert.size(), sizeof(lib::ModelVertex));//描画に使うモデル情報
			m_Shader->inputBufferSize(5, spring.size(), sizeof(SpringData));//バネモデル情報
			m_Shader->createUnorderdAccessView();

			m_Shader->mapOutput(0);
			m_Shader->mapOutput(1);
			m_Shader->mapOutput(2);
		}
		else m_Shader.reset(new lib::ComputeShader(m_ShaderHandler[m_Model_id].compute_handle, m_Dx12));
		m_Shader->mapInput(3, m_ShaderHandler[m_Model_id].sim_param.data());
		m_Shader->mapInput(4, vert.data());
		m_Shader->mapInput(5, spring.data());
		is_input = true;
		m_Shader->execution(
			m_ShaderHandler[m_Model_id].thread.x,
			m_ShaderHandler[m_Model_id].thread.y,
			m_ShaderHandler[m_Model_id].thread.z,
			m_ShaderHandler[m_Model_id].compute_handle
		);
		dataAssign(vert, spring);
	}

	void ClothWorldForce::dataAssign(std::vector<lib::ModelVertex>& vert, std::vector<SpringData>& spring) {
		vert.assign(
			(lib::ModelVertex*)m_Shader->getData(0),
			(lib::ModelVertex*)m_Shader->getData(0) + vert.size());
		spring.assign(
			(SpringData*)m_Shader->getData(1),
			(SpringData*)m_Shader->getData(1) + spring.size());
		m_ShaderHandler[m_Model_id].frame.assign(
			(PositionFrame*)m_Shader->getData(2),
			(PositionFrame*)m_Shader->getData(2) + m_ShaderHandler[m_Model_id].frame.size());
		if (spring[0].velocity.x == 0 &&
			spring[0].velocity.y == 0 &&
			spring[0].velocity.z == 0) {
			int error = 1;
		}
	}
	ClothWorldForce::PositionFrame ClothWorldForce::getFrame(int model_id) {
		return m_ShaderHandler[model_id].frame[0];
	}
}