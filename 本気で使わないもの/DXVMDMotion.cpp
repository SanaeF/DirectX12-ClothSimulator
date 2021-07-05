#include "DXVMDMotion.h"
#include <algorithm>
#include<d3dx12.h>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

void DXVMDMotion::LoadVMDFile(const char* filepath, const char* name) {
	auto fp = fopen(filepath, "rb");
	fseek(fp, 50, SEEK_SET);//最初の50バイトは飛ばしてOK
	unsigned int keyframeNum = 0;
	fread(&keyframeNum, sizeof(keyframeNum), 1, fp);

	vector<VMDKeyFrame> keyframes(keyframeNum);
	for (auto& keyframe : keyframes) {
		fread(keyframe.boneName, sizeof(keyframe.boneName), 1, fp);//ボーン名
		fread(&keyframe.frameNo, sizeof(keyframe.frameNo) +//フレーム番号
			sizeof(keyframe.location) +//位置(IKのときに使用予定)
			sizeof(keyframe.quaternion) +//クオータニオン
			sizeof(keyframe.bezier), 1, fp);//補間ベジェデータ
	}

	//VMDのキーフレームデータから、実際に使用するキーフレームテーブルへ変換
	for (auto& f : keyframes) {
		mKeyFrameData[f.boneName].emplace_back(KeyFrame(f.frameNo, XMLoadFloat4(&f.quaternion),
			XMFLOAT2((float)f.bezier[3] / 127.0f, (float)f.bezier[7] / 127.0f),
			XMFLOAT2((float)f.bezier[11] / 127.0f, (float)f.bezier[15] / 127.0f)));
	}

	keyMotion();
	
	boneMotion();

	treeBoneCopy();
}

float GetYFromXOnBezier(float x, const XMFLOAT2& a, const XMFLOAT2& b, uint8_t n) {
	if (a.x == a.y && b.x == b.y)return x;//計算不要
	float t = x;
	const float k0 = 1 + 3 * a.x - 3 * b.x;//t^3の係数
	const float k1 = 3 * b.x - 6 * a.x;//t^2の係数
	const float k2 = 3 * a.x;//tの係数

	//誤差の範囲内かどうかに使用する定数
	constexpr float epsilon = 0.0005f;

	for (int i = 0; i < n; ++i) {
		//f(t)求めまーす
		auto ft = k0 * t * t * t + k1 * t * t + k2 * t - x;
		//もし結果が0に近い(誤差の範囲内)なら打ち切り
		if (ft <= epsilon && ft >= -epsilon)break;

		t -= ft / 2;
	}
	//既に求めたいtは求めているのでyを計算する
	auto r = 1 - t;
	return t * t * t + 3 * t * t * r * b.y + 3 * t * r * r * a.y;
}

void DXVMDMotion::AnimationInit(DWORD startTime) {
	_startTime = startTime;
}

bool DXVMDMotion::Update(DWORD startTime) {
	auto elapsedTime = startTime - _startTime;
	unsigned int frameNo = 30 * (elapsedTime / 1000.0f);
	for (auto& bonemotion : mKeyFrameData) {
		auto node = getBoneNodeTable()[bonemotion.first];
		//合致するものを探す
		auto keyframes = bonemotion.second;
		auto rit = find_if(keyframes.rbegin(), keyframes.rend(), [frameNo](const KeyFrame& keyframe) {
			return keyframe.frameNo <= frameNo;
		});
		if (rit == keyframes.rend())continue;//合致するものがなければ飛ばす
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
		auto mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
			rotation *
			XMMatrixTranslation(pos.x, pos.y, pos.z);
		setBoneMatrices(&node, mat);
	}
	treeBoneCopy();
	return true;
}

void DXVMDMotion::keyMotion() {
	for (auto& motion : mKeyFrameData) {
		sort(motion.second.begin(), motion.second.end(),
			[](const KeyFrame& lval, const KeyFrame& rval) {
			return lval.frameNo <= rval.frameNo;
		});
	}
}

void DXVMDMotion::boneMotion() {
	for (auto& bonemotion : mKeyFrameData) {
		auto node = getBoneNodeTable()[bonemotion.first];
		auto& pos = node.startPos;
		auto mat =
			XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
			XMMatrixRotationQuaternion(bonemotion.second[0].quaternion) *
			XMMatrixTranslation(pos.x, pos.y, pos.z);
		setBoneMatrices(&node, mat);

	}
}
