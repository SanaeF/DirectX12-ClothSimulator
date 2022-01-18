#include "ClothSpring.h"
#include "../../DirectXLib/Source/VectorMath/VectorMath.h"
namespace phy {
	ClothSpring::ClothSpring(
		ClothForce& world_f,
		std::vector <lib::ModelVertex>& vert,
		std::vector<MassModel>& mass_model,
		std::vector<SpringData>& spring,
		std::vector<lib::ModelVertex>& pre_vert
	) {
		m_World_f = world_f;
		m_Vert= vert;
		m_Mass_model = mass_model;
		m_Spring = spring;
		m_Pre_vert = pre_vert;
	}
	void ClothSpring::execution(std::vector<SpringData>& spring) {
		for (int ite = 0; ite < m_Vert.size(); ite++){
			executionPoint(ite);
		}
		spring = m_Spring;
	}
	DirectX::XMFLOAT3 ClothSpring::CalcForce(
		DirectX::XMFLOAT3 pos1,
		DirectX::XMFLOAT3 pos2,
		float length,
		float constant,
		float damping,
		float k
	) {
		DirectX::XMFLOAT3 result(0, 0, 0);
		DirectX::XMFLOAT3 n = lib::VectorMath::subtract(pos1, pos2);
		float leg = lib::VectorMath::mulAdd(n, n);
		n = lib::VectorMath::normalize(n);//³‹K‰»2
		float f = (length - leg) * (k);
		if (f < 0)f = f * (damping);//L‚Ñ‚é’·‚³‚Å‚ ‚ê‚ÎŒ¸‘Þ
		else f = f * (constant);
		result = lib::VectorMath::scale(n, f);
		return result;
	}
	DirectX::XMFLOAT3 ClothSpring::StructuralSolver(int id) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		int id2;
		for (int ite = 0; ite < 4; ite++) {
			if (ite == 0)id2 = m_Mass_model[id].id0;
			if (ite == 1)id2 = m_Mass_model[id].id1;
			if (ite == 2)id2 = m_Mass_model[id].id2;
			if (ite == 3)id2 = m_Mass_model[id].id3;
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(m_Pre_vert[id].position, m_Pre_vert[id2].position);//‚Î‚Ë‚ÌŽ©‘R’·
			DirectX::XMFLOAT3 f = CalcForce(
				m_Vert[id].position,
				m_Vert[id2].position,
				Natulength,
				m_World_f.tension.stretch,
				m_World_f.tension.shrink,
				m_World_f.k
			);
			result = lib::VectorMath::add(result, f);
		}
		return result;
	}
	DirectX::XMFLOAT3 ClothSpring::CompressionSolver(int id) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		int id2;
		for (int ite = 0; ite < 4; ite++) {
			if (ite == 0)id2 = m_Mass_model[id].id0;
			if (ite == 1)id2 = m_Mass_model[id].id1;
			if (ite == 2)id2 = m_Mass_model[id].id2;
			if (ite == 3)id2 = m_Mass_model[id].id3;
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(m_Pre_vert[id].position, m_Pre_vert[id2].position);//‚Î‚Ë‚ÌŽ©‘R’·
			DirectX::XMFLOAT3 f = CalcForce(
				m_Vert[id].position,
				m_Vert[id2].position,
				Natulength,
				m_World_f.compress.stretch,
				m_World_f.compress.shrink,
				m_World_f.k
			);
			result = lib::VectorMath::add(result, f);
		}
		return result;
	}
	DirectX::XMFLOAT3 ClothSpring::ShareSolver(int id) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		int id2;
		for (int ite = 0; ite < 4; ite++) {
			if (ite == 0)id2 = m_Mass_model[id].id4;
			if (ite == 1)id2 = m_Mass_model[id].id5;
			if (ite == 2)id2 = m_Mass_model[id].id6;
			if (ite == 3)id2 = m_Mass_model[id].id7;
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(m_Pre_vert[id].position, m_Pre_vert[id2].position);//‚Î‚Ë‚ÌŽ©‘R’·
			DirectX::XMFLOAT3 f = CalcForce(
				m_Vert[id].position,
				m_Vert[id2].position,
				Natulength,
				m_World_f.share.stretch,
				m_World_f.share.shrink,
				m_World_f.k
			);
			result = lib::VectorMath::add(result, f);
		}
		return result;
	}
	DirectX::XMFLOAT3 ClothSpring::BendingSolver(int id) {
		DirectX::XMFLOAT3 result(0.f, 0.f, 0.f);
		int id2;
		for (int ite = 0; ite < 4; ite++) {
			if (ite == 0)id2 = m_Mass_model[id].id8;
			if (ite == 1)id2 = m_Mass_model[id].id9;
			if (ite == 2)id2 = m_Mass_model[id].id10;
			if (ite == 3)id2 = m_Mass_model[id].id11;
			if (id2 == -1)continue;
			float Natulength = lib::VectorMath::distance(m_Pre_vert[id].position, m_Pre_vert[id2].position);//‚Î‚Ë‚ÌŽ©‘R’·
			DirectX::XMFLOAT3 f = CalcForce(
				m_Vert[id].position,
				m_Vert[id2].position,
				Natulength,
				m_World_f.bend.stretch,
				m_World_f.bend.shrink,
				m_World_f.k
			);
			result = lib::VectorMath::add(result, f);
		}
		return result;
	}
	void ClothSpring::executionPoint(int id) {
		if (isFixed(m_Vert[id]))return;
		DirectX::XMFLOAT3 f(0.f, 0.f, 0.f);
		f = lib::VectorMath::add(f, StructuralSolver(id));
		f = lib::VectorMath::add(f, CompressionSolver(id));
		f = lib::VectorMath::add(f, ShareSolver(id));
		f = lib::VectorMath::add(f, BendingSolver(id));
		m_Spring[id].force = f;
	}
	inline bool ClothSpring::isFixed(lib::ModelVertex vert) {
		if (vert.color.x == 1.f &&
			vert.color.y == 0.f &&
			vert.color.z == 0.f)return true;
		return false;
	}
}