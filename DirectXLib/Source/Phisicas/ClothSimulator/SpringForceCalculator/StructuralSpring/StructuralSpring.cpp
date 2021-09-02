#include "StructuralSpring.h"
#include "../../../../VectorMath/VectorMath.h"
namespace phy {
	StructuralSpring::StructuralSpring(float shrink, float stretch):
		m_Shrink(shrink),
		m_Stretch(stretch)
	{
	}
	void StructuralSpring::solver(
		int vert_id,
		SpringData& spring_data,
		std::vector<lib::ModelParam>& vertex,
		std::vector<lib::ModelParam>& pre_vert,
		std::vector<std::vector<int>>& pre_index_id
	) {
		auto id = vert_id;
		auto dx = lib::VectorMath::subtract(vertex[id].position, pre_vert[id].position);
		for (int ite = 0; ite < 4; ite++) {
			auto id2 = pre_index_id[id][ite];
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(pre_vert[id].position, pre_vert[id2].position);//‚Î‚Ë‚ÌŽ©‘R’·
			auto f = calcForce(vertex[id], vertex[id2], Natulength, 15);
			spring_data.force = lib::VectorMath::add(spring_data.force, f);
		}
	}
	DirectX::XMFLOAT3 StructuralSpring::calcForce(
		lib::ModelParam& vert1,
		lib::ModelParam& vert2,
		float length,
		float constant
	) {
		auto n = lib::VectorMath::subtract(vert1.position, vert2.position);
		auto leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//³‹K‰»2
		float f = (length - leg) * constant;
		if (f >= 0)f *= m_Shrink;
		else f *= m_Stretch;
		return lib::VectorMath::scale(n, f);
	}
}