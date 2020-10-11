#include "DXVMDMotion.h"
#include<d3dx12.h>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

void DXVMDMotion::LoadVMDFile(const char* filepath, const char* name) {
	auto fp = fopen(filepath, "rb");
	fseek(fp, 50, SEEK_SET);//ç≈èâÇÃ50ÉoÉCÉgÇÕîÚÇŒÇµÇƒOK
	unsigned int motionDataNum = 0;
	fread(&motionDataNum, sizeof(motionDataNum), 1, fp);
	struct VMDmotionData {
		char boneName[15];
		unsigned int freameNo;
		XMFLOAT3 location;
		XMFLOAT4 quaternion;

		unsigned char bezier[64];
	};

	struct Motion {
		unsigned int frameNo;
		DirectX::XMVECTOR quaternion;
			//Motion();
		Motion(unsigned int fno, const XMVECTOR& q) 
			:frameNo(fno),
			quaternion(q)
		{
		}
	};

	std::unordered_map<std::string, std::vector<Motion>> mMotionData;

	std::vector<VMDmotionData>vmdMotionData(motionDataNum);
	for (auto& motion : vmdMotionData) {
		fread(motion.boneName, sizeof(motion.boneName), 1, fp);
		fread(
			&motion.freameNo,
			sizeof(motion.freameNo) +
			sizeof(motion.location) +
			sizeof(motion.quaternion),
			1,
			fp
		);

	}
	for (auto& vmdMotion : vmdMotionData) {
		mMotionData[vmdMotion.boneName].emplace_back(
			Motion(
				vmdMotion.freameNo,
				XMLoadFloat4(&vmdMotion.quaternion)
			)
		);
	}
	for (auto& bonemotion : mMotionData) {
		auto node = getBoneNodeTable()[bonemotion.first];
		auto& pos = node.startPos;
		auto mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
			XMMatrixRotationQuaternion(bonemotion.second[0].quaternion) *
			XMMatrixTranslation(pos.x, pos.y, pos.z);
		getBoneMatrices()[node.boneIdx] = mat;
	}
	BoneInitialize();
}

float GetYFromXOnBezier(float x, const XMFLOAT2& a, const XMFLOAT2& b, uint8_t n) {
	if (a.x == a.y && b.x == b.y)return x;//åvéZïsóv
	float t = x;
	const float k0 = 1 + 3 * a.x - 3 * b.x;//t^3ÇÃåWêî
	const float k1 = 3 * b.x - 6 * a.x;//t^2ÇÃåWêî
	const float k2 = 3 * a.x;//tÇÃåWêî

	//åÎç∑ÇÃîÕàÕì‡Ç©Ç«Ç§Ç©Ç…égópÇ∑ÇÈíËêî
	constexpr float epsilon = 0.0005f;

	for (int i = 0; i < n; ++i) {
		//f(t)ãÅÇﬂÇ‹Å[Ç∑
		auto ft = k0 * t * t * t + k1 * t * t + k2 * t - x;
		//Ç‡Çµåãâ Ç™0Ç…ãﬂÇ¢(åÎç∑ÇÃîÕàÕì‡)Ç»ÇÁë≈ÇøêÿÇË
		if (ft <= epsilon && ft >= -epsilon)break;

		t -= ft / 2;
	}
	//ä˘Ç…ãÅÇﬂÇΩÇ¢tÇÕãÅÇﬂÇƒÇ¢ÇÈÇÃÇ≈yÇåvéZÇ∑ÇÈ
	auto r = 1 - t;
	return t * t * t + 3 * t * t * r * b.y + 3 * t * r * r * a.y;
}

void DXVMDMotion::PlayAnimation() {
	_startTime = timeGetTime();
}

bool DXVMDMotion::Update(DWORD startTime) {
	auto elapsedTime = timeGetTime() - _startTime;
	unsigned int frameNo = 30 * (elapsedTime / 1000.0f);
	for (auto& bonemotion : mKeyFrameData) {
		auto node = getBoneNodeTable()[bonemotion.first];
		//çáívÇ∑ÇÈÇ‡ÇÃÇíTÇ∑
		auto keyframes = bonemotion.second;
		auto rit = find_if(keyframes.rbegin(), keyframes.rend(), [frameNo](const KeyFrame& keyframe) {
			return keyframe.frameNo <= frameNo;
		});
		if (rit == keyframes.rend())continue;//çáívÇ∑ÇÈÇ‡ÇÃÇ™Ç»ÇØÇÍÇŒîÚÇŒÇ∑
		XMMATRIX rotation;
		auto it = rit.base();
		if (it != keyframes.end()) {
			auto t = static_cast<float>(frameNo - rit->frameNo) /
				static_cast<float>(it->frameNo - rit->frameNo);
			t = GetYFromXOnBezier(t, it->p1, it->p2, 12);
			rotation = XMMatrixRotationQuaternion(
				XMQuaternionSlerp(rit->quaternion, it->quaternion, t)
			);
		}
		else {
			rotation = XMMatrixRotationQuaternion(rit->quaternion);
		}
		auto& pos = node.startPos;
		auto mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *//å¥ì_Ç…ñﬂÇµ
			rotation *//âÒì]
			XMMatrixTranslation(pos.x, pos.y, pos.z);//å≥ÇÃç¿ïWÇ…ñﬂÇ∑
		getBoneMatrices()[node.boneIdx] = mat;
	}
}