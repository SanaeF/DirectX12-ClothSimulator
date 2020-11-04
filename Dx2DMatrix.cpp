#include "Dx2DMatrix.h"

#include "Dx12Wrapper.h"


using namespace DirectX;

void Dx2DMatrix::createBuffer(Dx12Wrapper& DxWrap,int Handle) {
	mMatrix.resize(Handle + 1);
	pMapMatrix.resize(Handle + 1);
	mMatrix[Handle] = XMMatrixIdentity();

	mMatrix[Handle].r[0].m128_f32[0] = 2.0f / DxWrap.getPixelSize().cx;
	mMatrix[Handle].r[1].m128_f32[1] = -2.0f / DxWrap.getPixelSize().cy;
	//mMatrix[Handle].r[0];
	//mMatrix[Handle].r[1];
	RotaInitialize();
	DxWrap.Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(mMatrix) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mConstBuffer)
	);

	auto result = mConstBuffer->Map(0, nullptr, (void**)&pMapMatrix[Handle]);
	*pMapMatrix[Handle] = mWorlMat * mLookAt * mPerspectiveFov;
}

void Dx2DMatrix::Rotation(double RotaX, double RotaY, double RotaZ, int Handle) {
	//mWorlMat = XMMatrixRotationX(RotaX);
	//mWorlMat = XMMatrixRotationY(RotaY);
	mWorlMat = XMMatrixRotationZ(RotaZ);
	*pMapMatrix[Handle] = mWorlMat * mLookAt * mPerspectiveFov;

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
Dx2DMatrix::getConstBuffer(int Handle) {
	return mConstBuffer;
}