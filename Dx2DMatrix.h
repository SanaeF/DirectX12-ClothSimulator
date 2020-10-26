#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<map>
class Dx12Wrapper;

class Dx2DMatrix {
private:
	ID3D12Resource* mConstBuffer = nullptr;

	std::map<int, DirectX::XMMATRIX> mMatrix;

	std::map<int, DirectX::XMMATRIX*> pMapMatrix;

	DirectX::XMMATRIX mWorlMat;

	DirectX::XMMATRIX mLookAt;

	DirectX::XMMATRIX mPerspectiveFov;

public:

	void createBuffer(Dx12Wrapper& DxWrap, int Handle);

	void Rotation(double RotaX, double RotaY, double RotaZ, int Handle);

	ID3D12Resource* getConstBuffer(int Handle);

private:

	void RotaInitialize();

};