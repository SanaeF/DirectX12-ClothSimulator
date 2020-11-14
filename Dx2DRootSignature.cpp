#include "Dx2DRootSignature.h"

#include "Dx12Wrapper.h"

ID3D12RootSignature* Dx2DRootSignature::CreateRootSignature(Dx12Wrapper& DxWrap, D3D12_ROOT_SIGNATURE_DESC* mRootSigunatureDesuc) {
	//mRootSigBlob->Release();
	auto result = D3D12SerializeRootSignature(
		mRootSigunatureDesuc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&mRootSigBlob,
		&errorBlob
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "D3D12SerializeRootSignature Error!");
	}

	result = DxWrap.Device()->CreateRootSignature(
		0,
		mRootSigBlob->GetBufferPointer(),
		mRootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "CreateRootSignature Error!");
	}

	return mRootSignature.Get();
}

ID3D12RootSignature* Dx2DRootSignature::getRootSignature() {
	return mRootSignature.Get();
}