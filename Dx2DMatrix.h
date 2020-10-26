#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>
#include<DirectXTex.h>

class Dx12Wrapper;

class Dx2DMatrix {
private:

	ID3D12Resource* constBuffer = nullptr;

	DirectX::XMMATRIX mMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX* pMapMatrix;

public:

	void createBuffer(Dx12Wrapper& DxWrap);

private:



};