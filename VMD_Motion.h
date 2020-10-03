#pragma once
#include<DirectXMath.h>

class VMD_Motion {
private:

	struct KeyFrame {
		unsigned int frameNo;//フレーム№(アニメーション開始からの経過時間)
		DirectX::XMVECTOR quaternion;//クォータニオン
		DirectX::XMFLOAT2 p1, p2;//ベジェの中間コントロールポイント
		KeyFrame(unsigned int fno, DirectX::XMVECTOR& q, const DirectX::XMFLOAT2& ip1, const DirectX::XMFLOAT2& ip2) :
			frameNo(fno),
			quaternion(q),
			p1(ip1),
			p2(ip2) {
		};

	};

	

public:
	void LoadVMDFile(const char* filepath, const char* name);
};