#pragma once
#include<DirectXMath.h>
#include<unordered_map>
#include<map>

#include"DXPMDBone.h"

class DXVMDMotion : public DXPMDBone {
private:
	DWORD _startTime;

	struct VMDKeyFrame {
		char boneName[15]; // ボーン名
		unsigned int frameNo; // フレーム番号(読込時は現在のフレーム位置を0とした相対位置)
		DirectX::XMFLOAT3 location; // 位置
		DirectX::XMFLOAT4 quaternion; // Quaternion // 回転
		unsigned char bezier[64]; // [4][4][4]  ベジェ補完パラメータ
	};

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

public:
	void LoadVMDFile(const char* filepath, const char* name);

	bool Update(DWORD startTime);

	void PlayAnimation(DWORD _startTime);

	DXVMDMotion() {
	}
private:

	void keyMotion();
	void boneMotion();

};