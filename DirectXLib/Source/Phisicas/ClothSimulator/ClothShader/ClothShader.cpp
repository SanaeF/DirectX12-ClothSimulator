#include "ClothShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/VectorMath/VectorMath.h"

#include "ClothSpringShader/ClothSpringShader.h"
#include "ClothCollisionShader/ClothCollisionShader.h"

namespace phy {
	bool ClothShader::m_Is_simulated = false;
	ClothShader::ClothShader(std::shared_ptr<lib::DirectX12Manager>& dx_12):
		m_Dx12(dx_12)
	{

	}
	void ClothShader::execution(
		int model_id,
		int step,
		int time,
		lib::ModelData& model,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring_data
	) {
		massSpring(model_id, step, time, model, mass_model, spring_data);
		//collider(model_id, model, spring_data, mass_spring_id);
	}
	void ClothShader::massSpring(
		int model_id,
		int step,
		int time,
		lib::ModelData& model,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring_data
	) {
		//ステップ数だけバネの計算をする
		ClothSpringShader cloth_shader(model_id, m_Dx12);
		if (m_Is_simulated)worldForce(time, 0, model, spring_data);
		for (int ite = 0; ite < step; ite++) {
			cloth_shader.create(ite, model, mass_model, spring_data);
			cloth_shader.execution(m_Is_simulated, model, spring_data);
			forceZero(spring_data);
		}
		if (cloth_shader.isSimulated())m_Is_simulated = true;
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
	void ClothShader::worldForce(int time, int step, lib::ModelData& model, std::vector<SpringData>& spring_data) {
		WorldForce world_force;
		world_force.grid_mass = 1.f;
		world_force.gravity = 9.8f;
		world_force.damping = 1.3f;
		world_force.dt = 0.026;
		world_force.wind = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		for (int ite = 0; ite < spring_data.size(); ite++) {
			if (isFixed(model.vertex[ite]))continue;
			spring_data[ite].mass = world_force.grid_mass;
			//重力を加える
			spring_data[ite].force.y -= spring_data[ite].mass * world_force.gravity;
			//風力を加える
			double r1 = time / 10;
			spring_data[ite].force.x += world_force.wind.x * (sin(r1) * sin(r1) * 0.25 + 0.25);
			//ダンピング
			auto d = lib::VectorMath::scale(spring_data[ite].velocity, world_force.damping);
			spring_data[ite].force = lib::VectorMath::subtract(spring_data[ite].force, d);
			auto v = lib::VectorMath::scale(spring_data[ite].force, 1 / (spring_data[ite].mass + spring_data[ite].mass));
			v = lib::VectorMath::scale(v, world_force.dt);
			spring_data[ite].velocity = lib::VectorMath::add(spring_data[ite].velocity, v);
			v = lib::VectorMath::scale(spring_data[ite].velocity, world_force.dt);
			model.vertex[ite].position = lib::VectorMath::add(model.vertex[ite].position, v);
			//力をゼロにする
			spring_data[ite].force = DirectX::XMFLOAT3(0, 0, 0);
		}
	}
	void ClothShader::forceZero(std::vector<SpringData>& spring_data) {
		for (int ite = 0; ite < spring_data.size(); ite++) {
			spring_data[ite].force = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
			spring_data[ite].mass = 1.f;
		}
	}
	inline bool ClothShader::isNullty(DirectX::XMFLOAT3 pos) {
		if (pos.x == 0.f && pos.y == 0.f && pos.z == 0.f)return true;
		return false;
	}
	inline bool ClothShader::isFixed(lib::ModelVertex vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}