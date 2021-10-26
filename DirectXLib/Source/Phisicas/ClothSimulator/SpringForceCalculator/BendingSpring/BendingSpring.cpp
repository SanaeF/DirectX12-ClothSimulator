#include "BendingSpring.h"
#include "../../../../VectorMath/VectorMath.h"
namespace phy {
	BendingSpring::BendingSpring(float shrink, float stretch):
		m_Shrink(shrink),
		m_Stretch(stretch)
	{
	}
	void BendingSpring::solver(
		int vert_id,
		std::vector<SpringData>& spring_data,
		std::vector<lib::ModelVertex>& vertex,
		std::vector<lib::ModelVertex>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][4 + ite];
			if (id2 == -1)continue;
			for (int ite3 = 0; ite3 < 4; ite3++) {
				auto id3 = pre_index_id[ite][4 + ite3];
				if (id3 == -1 || id2 == id3)continue;
				float Natulength = lib::VectorMath::distance(pre_vert[id2].position, pre_vert[id3].position);//ばねの自然長
				auto f = calcForce(vertex[id2], vertex[id3], Natulength, bend_damping);
				auto size = Natulength - lib::VectorMath::distance(vertex[id].position, vertex[id2].position);
				if (sqrt(size * size) > Natulength) {
					auto n = lib::VectorMath::subtract(pre_vert[id].position, vertex[id].position);
					n = lib::VectorMath::normalize(n);//正規化2
					n = lib::VectorMath::scale(n, bend_damping);
					f = lib::VectorMath::add(f, n);
				}
				spring_data[id2].force = lib::VectorMath::add(spring_data[id2].force, f);
			}
		}
	}
	void BendingSpring::solver2(
		int vert_id,
		std::vector<SpringData>& spring_data,
		std::vector<lib::ModelVertex>& vertex,
		std::vector<lib::ModelVertex>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][4 + ite];
			if (id2 == -1)continue;
			for (int ite3 = 0; ite3 < 4; ite3++) {
				auto id3 = pre_index_id[ite][4 + ite3];
				if (id3 == -1 || id2 == id3)continue;
				float Natulength = lib::VectorMath::distance(pre_vert[id2].position, pre_vert[id3].position);//ばねの自然長
				auto f = calcForce(vertex[id2], vertex[id3], Natulength, bend);
				//ダンピング
				auto vect_vel = lib::VectorMath::subtract(spring_data[id2].velocity, spring_data[id].velocity);//速度ベクトル
				auto f_damp = lib::VectorMath::scale(vect_vel, bend_damping);
				f = lib::VectorMath::add(f, f_damp);
				//f = lib::VectorMath::scale(f, 1 / 3);
				spring_data[id2].force = lib::VectorMath::add(spring_data[id2].force, f);
			}
		}
	}
	DirectX::XMFLOAT3 BendingSpring::calcForce(
		lib::ModelVertex& vert1,
		lib::ModelVertex& vert2,
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