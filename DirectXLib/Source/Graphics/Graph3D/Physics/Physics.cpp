#include "Physics.h"
#include <iostream>
#include "../Vector3D/Vector3D.h"

namespace physlib {
	void Physics::setMass() {
		param.gravity *= param.mass;
		param.wind *= param.mass;
		param.constantSpring *= param.mass;
	}
	//void Physics::update() {
	//	lib::Vector3D vec;


	//	//êœï™
	//	param.force3.y -= param.gravity;
	//	param.force3.z *= param.wind * (sin(param.acc) * 0.5 + 0.5);
	//	param.force3 = vec.scale(param.force3, step * step * 0.5 / param.mass);

	//}
}