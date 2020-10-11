#pragma once
#include<DirectXMath.h>
#include<unordered_map>
#include<map>

#include"DXPMDBone.h"

class DXVMDMotion : public DXPMDBone {
private:
	DWORD _startTime;


	struct KeyFrame {
		unsigned int frameNo;//フレーム№(アニメーション開始からの経過時間)
		DirectX::XMVECTOR quaternion;//クォータニオン
		DirectX::XMFLOAT2 p1, p2;//ベジェの中間コントロールポイント
		KeyFrame(unsigned int fno, const DirectX::XMVECTOR& q, const DirectX::XMFLOAT2& ip1, const DirectX::XMFLOAT2& ip2) :
			frameNo(fno),
			quaternion(q),
			p1(ip1),
			p2(ip2) {}
	};
	std::unordered_map<std::string, std::vector<KeyFrame>> mKeyFrameData;

	void keyMotion();

	void boneMotion();

public:
	void LoadVMDFile(const char* filepath, const char* name);

	bool Update(
		DWORD startTime
	);

	void PlayAnimation();

	DXVMDMotion() {
	}

};