#include "Dx2DMatrix.h"

#include "Dx12Wrapper.h"


using namespace DirectX;

void Dx2DMatrix::createBuffer(Dx12Wrapper& DxWrap) {
	RotaInitialize();
	int instNum = 30;

	mMatrix = XMMatrixIdentity();
	mMatrix.r[0].m128_f32[0] = 2.0f / DxWrap.getPixelSize().cx;
	mMatrix.r[1].m128_f32[1] = -2.0f / DxWrap.getPixelSize().cy;

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper\‘¢‘Ì
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(mMatrix) * instNum + 0xff) & ~0xff);//Helper\‘¢‘Ì
	DxWrap.Device()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mConstBuffer)
	);
	auto result = mConstBuffer->Map(0, nullptr, (void**)&mMapMatrix);

}

void Dx2DMatrix::ChangeMatrix(
	int InstID,
	DirectX::XMMATRIX* pMatrix,
	float x,
	float y,
	float size,
	float RotaZ
) {
	float Angler = 0.0f;

	XMVECTOR PosisitonOrigin = XMVectorSet(x, y, 0, 0);
	mWorlMat = DirectX::XMMatrixTransformation2D(
		PosisitonOrigin,
		0,
		XMVectorSet(size, size, 0, 0),
		PosisitonOrigin,
		RotaZ,
		PosisitonOrigin
	);
	for (int i = 0; i < 30; i++) {
		float addRand = 0.0f;
		if (i == 0) {
			PosisitonOrigin = XMVectorSet(x, y, 0, 0);
		}
		else {
			if (i % 2 == 1)addRand = static_cast <float>(-100 + (rand() % 200)) / static_cast <float>(1440);
			else addRand = 0.0f;
			if (i <= 10) {
				PosisitonOrigin = XMVectorSet(x - static_cast <float>(200 * i) / static_cast <float>(1920), y + addRand, 0, 0);
			}
			else {
				PosisitonOrigin = XMVectorSet(x + static_cast <float>(120 * (i - 10)) / static_cast <float>(1920), y + addRand, 0, 0);
			}
			
		}
		if (i % 2 == 1)Angler = -RotaZ;
		else Angler = RotaZ;
		mWorlMat = DirectX::XMMatrixTransformation2D(
			PosisitonOrigin,
			0,
			XMVectorSet(size, size, 0, 0),
			XMVectorSet(0, 0, 0, 0),
			Angler,
			PosisitonOrigin
		);
		//mWorlMat = (TransMat * SizeMat * RotaMat);
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