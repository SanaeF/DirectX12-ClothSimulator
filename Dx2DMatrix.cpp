#include "Dx2DMatrix.h"

#include "Dx12Wrapper.h"


using namespace DirectX;

void Dx2DMatrix::createBuffer(Dx12Wrapper& DxWrap) {
	RotaInitialize();
	for (int i = 0; i < 10; i++) {
		mMatrix[i] = XMMatrixIdentity();
		mMatrix[i].r[0].m128_f32[0] = 2.0f / DxWrap.getPixelSize().cx;
		mMatrix[i].r[1].m128_f32[1] = -2.0f / DxWrap.getPixelSize().cy;

	}
	DxWrap.Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(*mMatrix) * 10 + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mConstBuffer)
	);
	auto result = mConstBuffer->Map(0, nullptr, (void**)&mMapMatrix);
	for (int i = 0; i < 10; i++) {
		mMapMatrix[i] = mLookAt * mPerspectiveFov;
	}
}

void Dx2DMatrix::ChangeMatrix(
	int InstID,
	DirectX::XMMATRIX* pMatrix,
	float x,
	float y,
	float size,
	float RotaZ
) {
	DirectX::XMMATRIX TransMat = XMMatrixTranslation(x, y, 0);
	DirectX::XMMATRIX SizeMat = XMMatrixScaling(size, size, size);
	DirectX::XMMATRIX RotaMat = XMMatrixRotationZ(RotaZ);
	mWorlMat = TransMat * SizeMat * RotaMat;

	for (int i = 0; i < 10; i++) {
		if (i == 0) {
			SizeMat = XMMatrixScaling(size, size, size);
			RotaMat = XMMatrixRotationZ(RotaZ);
			TransMat = XMMatrixTranslation(x, y, 0);
		}
		else {
			SizeMat = XMMatrixScaling(size, size, size);
			
			if (i <= 5) {
				RotaMat = XMMatrixRotationZ(-RotaZ);
				TransMat = XMMatrixTranslation(x - static_cast <float>(350 * i) / static_cast <float>(1920), y, 0);
			}
			else {
				RotaMat = XMMatrixRotationZ(RotaZ);
				TransMat = XMMatrixTranslation(x + static_cast <float>(350 * (i - 5)) / static_cast <float>(1920), y, 0);
			}
		}
		mWorlMat = TransMat * SizeMat * RotaMat;
		pMatrix[i] = mWorlMat * mLookAt * mPerspectiveFov;
	}
}

void Dx2DMatrix::Rotation(
	int InstID,
	DirectX::XMMATRIX& pMatrix,
	double RotaX,
	double RotaY,
	double RotaZ
) {
	mWorlMat = XMMatrixScaling(1 - (InstID / 1000), 1 - (InstID / 1000), 1-(InstID / 1000));
	//mWorlMat = XMMatrixRotationZ(RotaZ) * InstID;
	pMatrix = mWorlMat * mLookAt * mPerspectiveFov;
	//pMapMatrix = &pMatrix;
}

void Dx2DMatrix::TransMove(
	DirectX::XMMATRIX& pMatrix,
	double RotaX,
	double RotaY,
	double RotaZ
) {
	mWorlMat = XMMatrixTranslation(RotaX, RotaY, RotaZ);

	pMatrix = mWorlMat;
}

void Dx2DMatrix::RotaInitialize() {
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);

	mLookAt = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
	mPerspectiveFov = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast < float>(1920)/ static_cast < float>(1440),
		1.f,
		10.f
	);
}

ID3D12Resource* 
Dx2DMatrix::getConstBuffer() {
	return mConstBuffer;
}

DirectX::XMMATRIX* 
Dx2DMatrix::getMatData() {

	return mMapMatrix;
}