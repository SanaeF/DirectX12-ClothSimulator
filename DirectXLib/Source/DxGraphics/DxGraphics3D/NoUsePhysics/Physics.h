#pragma once
#include <DirectXMath.h>

namespace physlib {
	class Physics {
	private:
		struct Resistance {
			float stretch;
			float shrink;
		};
		struct Spring {
			float constant;
			Resistance structural;
			Resistance shear;
			Resistance bending;
		};
		struct Paramater {
			DirectX::XMFLOAT3 force3;
			float force2;
			float gravity;
			float resistance;
			float relaxation;
			float mass;
			float wind;
			float acc;
			float constantSpring;
			Spring spring;
		};
	public:
		Paramater param;
		void setMass();
		//ばねの弾性力(いわゆるf=kx)。
		static float getSpringForce(
			float k,
			float fixity_point, float moved_point,
			float pre_fixity_point, float pre_moved_point
		);
		//ばねの弾性力(いわゆるf=k*vector_length)を3次元で成分表示して計算してみる。
		static DirectX::XMFLOAT3 getSpringForce3(
			float k,
			DirectX::XMFLOAT3 fixity_point, DirectX::XMFLOAT3 moved_point,
			DirectX::XMFLOAT3 pre_fixity_point, DirectX::XMFLOAT3 pre_moved_point
		);
	};
}