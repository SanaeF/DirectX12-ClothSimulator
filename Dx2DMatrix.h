#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<map>
class Dx12Wrapper;

class Dx2DMatrix {
private:
	ID3D12Resource* mConstBuffer = nullptr;

	//ID3D12Resource* mInstBuffer = nullptr;

	DirectX::XMMATRIX* mConstMatrix;
	DirectX::XMMATRIX mMatrix;
	DirectX::XMMATRIX* mMapMatrix;

	DirectX::XMMATRIX mWorlMat;

	DirectX::XMMATRIX mLookAt;

	DirectX::XMMATRIX mPerspectiveFov;

	D3D12_SHADER_RESOURCE_VIEW_DESC mInstSRV_Desc = {};

public:


	void createBuffer(Dx12Wrapper& DxWrap);

	void ChangeMatrix(
		DirectX::XMMATRIX* pMatrix,
		float x,
		float y,
		float size,
		float RotaZ
	);
	ID3D12Resource* getConstBuffer();

	DirectX::XMMATRIX* getMatData();

private:

	void RotaInitialize(Dx12Wrapper& DxWrap);

};