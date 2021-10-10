#include "ClothShader.h"
#include "ClothSpringShader/ClothSpringShader.h"
#include "ClothCollisionShader/ClothCollisionShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"

namespace phy {
	ClothShader::ClothShader(std::shared_ptr<lib::DirectX12Manager>& dx_12):
		m_Dx12(dx_12)
	{

	}
	void ClothShader::execution(
		int model_id,
		int step,
		int time,
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		massSpring(model_id, step, time, model, spring_data, mass_spring_id);
		forceZero(spring_data);
		collider(model_id, model, spring_data, mass_spring_id);
	}
	void ClothShader::massSpring(
		int model_id,
		int step,
		int time,
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		//ステップ数だけバネの計算をする
		ClothSpringShader cloth_shader(model_id, m_Dx12);
		for (int ite = 0; ite < step; ite++) {
			cloth_shader.create(model, spring_data, mass_spring_id);
			cloth_shader.execution(step);
			cloth_shader.dataChange(model_id, model, spring_data);
		}
	}
	void ClothShader::collider(
		int model_id,
		lib::ModelData& model,
		std::vector<SpringData>& spring_data,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		auto param = ClothSpringShader::getMaxMinPos(model_id);
		if (isNullty(param.min_pos) && isNullty(param.max_pos))return;
		//当たり判定の計算をする
		ClothCollisionShader collision(model_id, param.max_pos, param.min_pos, m_Dx12);
		collision.create(model, spring_data, mass_spring_id);
		collision.execution();
		collision.dataChange(model_id, model, spring_data);
	}
	void ClothShader::forceZero(std::vector<SpringData>& spring_data) {
		for (int ite = 0; ite < spring_data.size(); ite++)spring_data[ite].force = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}
	bool ClothShader::isNullty(DirectX::XMFLOAT3 pos) {
		if (pos.x == 0.f && pos.y == 0.f && pos.z == 0.f)return true;
		return false;
	}
}