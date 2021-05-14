#include "DxGraph2DIndex.h"
#include "../../../DirectX12/DirectX12Manager.h"

namespace libGraph {
	void DxIndex2D::CreateBuffer(lib::DirectX12Manager& DxWrap) {
		mCreateBuff(DxWrap);

		D3D12_RESOURCE_DESC mResource_desc = {};
		mResource_desc.Width = sizeof(index);
		setResourceDesc(mResource_desc);
		auto heapProp = getHeap_prop();
		auto resDesc = getResource_desc();
		auto result = DxWrap.Device()->CreateCommittedResource(//
			&heapProp,//
			D3D12_HEAP_FLAG_NONE,//
			&resDesc,//
			D3D12_RESOURCE_STATE_GENERIC_READ,//
			nullptr,
			IID_PPV_ARGS(&mIndex_Buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "CreateCommittedResource");
		}

	}

	void DxIndex2D::CopyBuffer() {
		unsigned short* mappedIdx = nullptr;

		mIndex_Buff->Map(0, nullptr, (void**)&mappedIdx);
		std::copy(
			std::begin(index),
			std::end(index),
			mappedIdx
		);
		mIndex_Buff->Unmap(0, nullptr);

	}

	void DxIndex2D::CreateIndexBufferView(lib::DirectX12Manager& DxWrap) {
		CreateBuffer(DxWrap);
		CopyBuffer();
		mIndexView.BufferLocation = mIndex_Buff->GetGPUVirtualAddress();
		mIndexView.Format = DXGI_FORMAT_R16_UINT;
		mIndexView.SizeInBytes = sizeof(index);
	}

	D3D12_INDEX_BUFFER_VIEW
		DxIndex2D::getIndexView() {
		return mIndexView;
	}
}