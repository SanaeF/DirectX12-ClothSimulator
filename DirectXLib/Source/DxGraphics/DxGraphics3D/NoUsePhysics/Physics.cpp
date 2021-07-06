#include "Physics.h"
#include <iostream>
#include "../Vector3D/Vector3D.h"

namespace physlib {
	void Physics::setMass() {
		param.gravity *= param.mass;
		param.wind *= param.mass;
		param.constantSpring *= param.mass;
	}
	float Physics::getSpringForce(
		float k,
		float fixity_point, float moved_point,
		float pre_fixity_point, float pre_moved_point
	) {
		//‚Î‚Ë‚Ì©‘R’·‚ğŒvZ
		auto natural_dist = pre_fixity_point - pre_moved_point;
		auto natural_length = sqrt(natural_dist * natural_dist);//â‘Î’l
		//L‚Ñ‚½Œã‚Ì‚Î‚Ë‚Ì’·‚³‚ğŒvZ
		auto dist = fixity_point - moved_point;
		auto length = sqrt(dist * dist);//â‘Î’l
		//‚Î‚Ë‚ÌL‚Ñ‚½’·‚³
		auto x = dist - natural_dist;
		return -x * k;
	}
	DirectX::XMFLOAT3 
	Physics::getSpringForce3(
		float k,
		DirectX::XMFLOAT3 fixity_point, DirectX::XMFLOAT3 moved_point,
		DirectX::XMFLOAT3 pre_fixity_point, DirectX::XMFLOAT3 pre_moved_point
	) {
		DirectX::XMFLOAT3 result;
		//x¬•ª‚ÌŒvZ
		result.x = getSpringForce(k, fixity_point.x, moved_point.x, pre_fixity_point.x, pre_moved_point.x);
		//y¬•ª‚ÌŒvZ
		result.y = getSpringForce(k, fixity_point.y, moved_point.y, pre_fixity_point.y, pre_moved_point.y);
		//z¬•ª‚ÌŒvZ
		result.z = getSpringForce(k, fixity_point.z, moved_point.z, pre_fixity_point.z, pre_moved_point.z);
		return result;
	}
}