#include "VMD_Motion.h"
#include"PMDRenderer.h"
#include"Dx12Wrapper.h"
#include<d3dx12.h>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

void VMD_Motion::LoadVMDFile(const char* filepath, const char* name) {
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

	std::unordered_map<std::string, std::vector<Motion>> _motiondata;

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
		_motiondata[vmdMotion.boneName].emplace_back(
			Motion(
				vmdMotion.freameNo,
				XMLoadFloat4(&vmdMotion.quaternion)
			)
		);
	}
}