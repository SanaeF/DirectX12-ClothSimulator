#include "Dx2DMatrix.h"

#include "Dx12Wrapper.h"

void Dx2DMatrix::createBuffer(Dx12Wrapper& DxWrap) {
	mMatrix = DirectX::XMMatrixIdentity();

	DxWrap.Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(mMatrix) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer)
	);

	auto result = constBuffer->Map(0, nullptr, (void**)&pMapMatrix);
	*pMapMatrix = mMatrix;

}