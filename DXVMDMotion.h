#pragma once
#include<DirectXMath.h>
#include<unordered_map>
#include<map>

#include"DXPMDBone.h"

class DXVMDMotion : public DXPMDBone {
private:
	DWORD _startTime;

	struct BoneNode {
		int boneIdx;//ボーンインデックス
		DirectX::XMFLOAT3 startPos;//ボーン基準点(回転中心)
		DirectX::XMFLOAT3 endPos;
		std::vector<BoneNode*> children;//子ノード
	};

	struct KeyFrame {
		unsigned int frameNo;//フレーム№(アニメーション開始からの経過時間)
		DirectX::XMVECTOR quaternion;//クォータニオン
		DirectX::XMFLOAT2 p1, p2;//ベジェの中間コントロールポイント
		KeyFrame(
			unsigned int fno, 
			DirectX::XMVECTOR& q,
			const DirectX::XMFLOAT2& ip1,
			const DirectX::XMFLOAT2& ip2
		) :
			frameNo(fno),
			quaternion(q),
			p1(ip1),
			p2(ip2) {
		};

	};
	std::unordered_map<std::string, std::vector<KeyFrame>> mKeyFrameData;


public:
	void LoadVMDFile(const char* filepath, const char* name);

	bool Update(
		DWORD startTime
	);

	void PlayAnimation();

	DXVMDMotion() {
	}

};