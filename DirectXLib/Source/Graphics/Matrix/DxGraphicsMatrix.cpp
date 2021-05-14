#include "DxGraphicsMatrix.h"

#include "../../DirectX12/DirectX12Manager.h"


using namespace DirectX;

namespace libGraph {
	void Dx2DMatrix::createBuffer(lib::DirectX12Manager& DxWrap) {
		RotaInitialize(DxWrap);
		int instNum = 30;
		mMatrix = XMMatrixIdentity();
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(mMatrix) + 0xff) & ~0xff);//Helper構造体
		DxWrap.Device()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mConstBuffer)
		);
		auto result = mConstBuffer->Map(0, nullptr, (void**)&mMapMatrix);
		*mMapMatrix = mMatrix * mLookAt * mPerspectiveFov;
		mConstBuffer->Unmap(0, nullptr);
	}
	void Dx2DMatrix::createBuffer3D(lib::DirectX12Manager& DxWrap) {
		RotaInitialize(DxWrap);
		int instNum = 30;
		DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(XM_PI);
		mMatrix = XMMatrixIdentity();
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatricesData) + 0xff) & ~0xff);//Helper構造体
		DxWrap.Device()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mConstBuffer)
		);
		auto result = mConstBuffer->Map(0, nullptr, (void**)&mapMatrix);
		mapMatrix->world = mMatrix;
		mapMatrix->viewproj = mLookAt * mPerspectiveFov;
		//mConstBuffer->Unmap(0, nullptr);
	}
	void Dx2DMatrix::ChangeMatrix(
		DirectX::XMMATRIX* pMatrix,
		float x,
		float y,
		float size,
		float RotaZ
	) {
		mWorlMat = DirectX::XMMatrixTransformation2D(
			XMVectorSet(0, 0, 0, 0),
			0,
			XMVectorSet(size, size, 0, 0),
			XMVectorSet(0, 0, 0, 0),
			RotaZ,
			XMVectorSet(x, y, 0, 0)
		);
		*pMatrix = mWorlMat * mLookAt * mPerspectiveFov;
	}

	void Dx2DMatrix::ChangeMatrix3D(
		DirectX::XMMATRIX* pMatrix,
		float x,
		float y,
		float size,
		float RotaZ
	) {
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(-6, -y, -x);
		//DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(XM_PI/6);
		DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(XM_PI / 2);
		DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(XM_PI);
		DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(size, size, size);
		mWorlMat = translate * rotate_y * rotate_z;
		mapMatrix->world = mWorlMat;
	}


	void Dx2DMatrix::RotaInitialize(lib::DirectX12Manager& DxWrap) {
		XMFLOAT3 eye(0, 0, -1);
		XMFLOAT3 target(0, 0, 0);
		XMFLOAT3 up(0, 1, 0);
		float pixW = static_cast<float>(DxWrap.getPixelSize().cx);
		float pixH = static_cast<float>(DxWrap.getPixelSize().cy);
		mLookAt = XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up)
		);
		mPerspectiveFov = XMMatrixPerspectiveFovLH(
			XM_PIDIV2,
			pixW / pixH,
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
}