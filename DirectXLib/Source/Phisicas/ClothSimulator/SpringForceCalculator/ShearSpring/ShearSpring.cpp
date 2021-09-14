#include "ShearSpring.h"
#include "../../../../VectorMath/VectorMath.h"
namespace phy {
	ShearSpring::ShearSpring(float shrink, float stretch):
		m_Shrink(shrink),
		m_Stretch(stretch)
	{
	}
	void ShearSpring::solver(
		int vert_id,
		SpringData& spring_data,
		std::vector<lib::ModelParam>& vertex,
		std::vector<lib::ModelParam>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][4 + ite];
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(pre_vert[id].position, pre_vert[id2].position);//ばねの自然長
			auto f = calcForce(vertex[id], vertex[id2], Natulength, shear);
			auto size = Natulength - lib::VectorMath::distance(vertex[id].position, vertex[id2].position);
			if (sqrt(size * size) > Natulength) {
				size = lib::VectorMath::distance(pre_vert[id].position, vertex[id].position);
				auto n = lib::VectorMath::subtract(pre_vert[id].position, vertex[id].position);
				n = lib::VectorMath::normalize(n);//正規化2
				n = lib::VectorMath::scale(n, size * shear_damping);
				f = lib::VectorMath::add(f, n);
			}
			spring_data.force = lib::VectorMath::add(spring_data.force, f);
		}
	}
	void ShearSpring::solver2(
		int vert_id,
		std::vector<SpringData>& spring_data,
		std::vector<lib::ModelParam>& vertex,
		std::vector<lib::ModelParam>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][4 + ite];
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(pre_vert[id].position, pre_vert[id2].position);//ばねの自然長
			auto f = calcForce(vertex[id], vertex[id2], Natulength, shear);
			//ダンピング
			auto vect_vel = lib::VectorMath::subtract(spring_data[id2].velocity, spring_data[id].velocity);//速度ベクトル
			auto f_damp = lib::VectorMath::scale(vect_vel, shear_damping);

			f = lib::VectorMath::add(f, f_damp);
			spring_data[id].force = lib::VectorMath::add(spring_data[id].force, f);
		}
	}
	DirectX::XMFLOAT3 ShearSpring::calcForce(
		lib::ModelParam& vert1,
		lib::ModelParam& vert2,
		float length,
		float constant
	) {
		auto n = lib::VectorMath::subtract(vert1.position, vert2.position);
		auto leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//正規化2
		float f = (length - leg) * constant;
		return lib::VectorMath::scale(n, f);
	}
}