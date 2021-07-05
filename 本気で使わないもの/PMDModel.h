#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>

class Dx12Wrapper;

class PMDModel {
private:
	//ハンドルに入れるべきデータ
	ID3D12Resource* VertexBuff;

	//struct PMDVertex {
	//	DirectX::XMFLOAT3 pos;
	//	DirectX::XMFLOAT3 normal;
	//	DirectX::XMFLOAT2 uv;
	//	unsigned short boneNo[2];
	//	unsigned char boneWeight;
	//	unsigned char edgeFlg;
	//};

public:

	int Load(std::shared_ptr<Dx12Wrapper> DxWrap);

	PMDModel();
	~PMDModel();

private:

	int LoadHeader(FILE* fp);

};