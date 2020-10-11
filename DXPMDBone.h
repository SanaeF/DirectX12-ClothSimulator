#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<unordered_map>
#include<wrl.h>

class DXPMDBone {
private:
	struct BoneNode {
		int boneIdx;//ボーンインデックス
		DirectX::XMFLOAT3 startPos;//ボーン基準点(回転中心)
		DirectX::XMFLOAT3 endPos;
		std::vector<BoneNode*> children;//子ノード
	};

	struct Transform {
		//内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
		//Transformをnewする際には16バイト境界に確保する
		void* operator new(size_t size);
		DirectX::XMMATRIX world;
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

	std::vector<DirectX::XMMATRIX> _boneMatrices;
	std::map<std::string, BoneNode> _boneNodeTable;

	DirectX::XMMATRIX* _mappedMatrices = nullptr;

	Transform _transform;

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> _transformBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> _transformHeap = nullptr;//座標変換ヒープ

public:
	void BoneInitialize();

	std::vector<DirectX::XMMATRIX> getBoneMatrices() {
		return _boneMatrices;
	}
	std::map<std::string, BoneNode> getBoneNodeTable() {
		return _boneNodeTable;
	}

	HRESULT CreateTransformView(ComPtr<ID3D12Device> _dev);

	ComPtr<ID3D12DescriptorHeap> getTransHeap();

	void setTransWorld();

	DXPMDBone() {

	}
private:

	void RecursiveMatrixMultiply(BoneNode* node, const DirectX::XMMATRIX& mat);

};