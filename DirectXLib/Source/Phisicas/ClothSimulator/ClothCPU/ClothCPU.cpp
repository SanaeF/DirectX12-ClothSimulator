#include "ClothCPU.h"
#include "ClothSpring/ClothSpring.h"
#include "ClothNewPosition/ClothNewPosition.h"
#include "../../DirectXLib/Source/VectorMath/VectorMath.h"
namespace phy {
	void ClothCPU::execution(
		int step,
		int time,
		ClothForce& world_f,
		lib::ModelData& model,
		std::vector<lib::ModelVertex>& pre_vert,
		std::vector<lib::ModelVertex>& last_vertex,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring_data,
		std::vector<PolygonModel>& polygon_model
	) {
		if (model.vertex.size() <= 0)return;
		worldForce(time, step, world_f, model, spring_data);
		if (world_f.collision_type == ClothForce::COLLISION_TYPE::OUT_STEP) {
			last_vertex = model.vertex;//過去の座標として記録
			for (int ite = 0; ite < step; ite++) {//ステップ数
				//クロスシミュレーションシェーダーの実行
				ClothSpring cloth(world_f, model.vertex, mass_model, spring_data, pre_vert);
				cloth.execution(spring_data);
				ClothNewPosition new_pos(world_f, model.vertex, spring_data, polygon_model);
				new_pos.execution(model.vertex, spring_data, false);//座標を変更
			}
			//auto param = new_pos.getFrame();
			if (world_f.is_self_collision) {//当たり判定の計算をする
				//collision.executeSortShader(model, param.max_pos, param.min_pos);//空間分割シェーダー
				////自己衝突シェーダー
				//collision.create(param.max_pos, param.min_pos, model, world_f, spring_data, polygon_model, pre_vert, last_vertex);
				//collision.execution(spring_data);
				//new_pos.execution(world_f, model.vertex, spring_data, polygon_model, true);//座標を変更
			}
		}
		if (world_f.collision_type == ClothForce::COLLISION_TYPE::IN_STEP) {
			for (int ite = 0; ite < step; ite++) {//ステップ数
				last_vertex = model.vertex;//過去の座標として記録
				//クロスシミュレーションシェーダーの実行
				ClothSpring cloth(world_f, model.vertex, mass_model, spring_data, pre_vert);
				cloth.execution(spring_data);
				ClothNewPosition new_pos(world_f, model.vertex, spring_data, polygon_model);
				new_pos.execution(model.vertex, spring_data, false);//座標を変更
				//auto param = new_pos.getFrame();
				if (world_f.is_self_collision) {//当たり判定の計算をする
					//collision.executeSortShader(model, param.max_pos, param.min_pos);//空間分割シェーダー
					////自己衝突シェーダー
					//collision.create(param.max_pos, param.min_pos, model, world_f, spring_data, polygon_model, pre_vert, last_vertex);
					//collision.execution(spring_data);
					//new_pos.execution(world_f, model.vertex, spring_data, polygon_model, true);//座標を変更
				}
			}
		}
	}
	void ClothCPU::worldForce(int time, int step, ClothForce& world_f, lib::ModelData& model, std::vector<SpringData>& spring_data) {
		auto world_force = world_f;
		world_force.dt = world_force.dt / 1000;//シェーダーのdtの100分の1
		for (int ite = 0; ite < spring_data.size(); ite++) {
			if (isFixed(model.vertex[ite]))continue;
			spring_data[ite].mass = world_force.grid_mass;
			//重力を加える
			spring_data[ite].force.y -= spring_data[ite].mass * world_force.gravity;
			//風力を加える
			double r1 = time / 30;
			spring_data[ite].force = lib::VectorMath::add(spring_data[ite].force, lib::VectorMath::scale(world_force.wind, sin(r1) * sin(r1) * 0.25f + 0.25f));
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
	inline bool ClothCPU::isFixed(lib::ModelVertex vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}