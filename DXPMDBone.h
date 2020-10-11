#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<unordered_map>
#include<wrl.h>

class DXPMDBone {
public:
	struct BoneNode {
		int boneIdx;//ボーンインデックス
		DirectX::XMFLOAT3 startPos;//ボーン基準点(回転中心)
		DirectX::XMFLOAT3 endPos;
		std::vector<BoneNode*> children;//子ノード
	};
private:

	struct Transform {
		//内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
		//Transformをnewする際には16バイト境界に確保する
		void* operator new(size_t size);
		DirectX::XMMATRIX world;
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
	void RecursiveMatrixMultiply(BoneNode* node, const DirectX::XMMATRIX& mat);

	void BoneLoad(FILE* fp);

	std::vector<DirectX::XMMATRIX> getBoneMatrices() {
		return _boneMatrices;
	}
	std::map<std::string, BoneNode> getBoneNodeTable() {
		return _boneNodeTable;
	}

	void setBoneMatrices(BoneNode* node, DirectX::XMMATRIX mat);

	HRESULT CreateTransformView(ComPtr<ID3D12Device> _dev);

	ComPtr<ID3D12DescriptorHeap> getTransHeap();

	void setTransWorld();

	DXPMDBone() {

	}

};