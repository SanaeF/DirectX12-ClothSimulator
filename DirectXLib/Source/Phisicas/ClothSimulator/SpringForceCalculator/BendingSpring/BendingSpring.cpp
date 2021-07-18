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
		std::vector<lib::ModelData>& vertex,
		std::vector<lib::ModelData>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][ite];
			if (id2 == -1)continue;
			for (int ite3 = 0; ite3 < 4; ite3++) {
				auto id3 = pre_index_id[ite][4 + ite3];
				if (id3 == -1 || id2 == id3)continue;
				float Natulength = lib::VectorMath::distance(pre_vert[id2].position, pre_vert[id3].position);//‚Î‚Ë‚ÌŽ©‘R’·
				auto f = calcForce(vertex[id2], vertex[id3], Natulength, 150);
				f = lib::VectorMath::scale(f, 1 / 2);
				spring_data[id2].force = lib::VectorMath::add(spring_data[id2].force, f);
			}
		}
	}
	DirectX::XMFLOAT3 BendingSpring::calcForce(
		lib::ModelData& vert1,
		lib::ModelData& vert2,
		float length,
		float constant
	) {
		auto n = lib::VectorMath::subtract(vert1.position, vert2.position);
		auto leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//³‹K‰»2
		float f = (length - leg) * constant;
		f >= 0 ? f *= m_Shrink : f *= m_Stretch;
		return lib::VectorMath::scale(n, f);
	}
}