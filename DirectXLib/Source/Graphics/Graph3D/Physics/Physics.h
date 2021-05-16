#pragma once
#include <DirectXMath.h>

namespace physlib {
	class Physics {
	private:
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
		};
	public:
		Paramater param;
		void setMass();
		//void update(int step);
	};
}