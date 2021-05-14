#include "DxGraphicsIndex.h"
#include "../../../../DirectX12/DirectX12Manager.h"

namespace libGraph {
	void DxIndexProp::CreateIndexBufferView(lib::DirectX12Manager& DxWrap, std::vector<Vertex> vert) {
		CreateBuffer(DxWrap, vert);
		CopyBuffer();
		mIndexView.BufferLocation = mIndex_Buff->GetGPUVirtualAddress();
		mIndexView.Format = DXGI_FORMAT_R16_UINT;
		mIndexView.SizeInBytes = sizeof(index);
	}
	void DxIndexProp::CreateBuffer(lib::DirectX12Manager& DxWrap, std::vector<Vertex> vert) {
		mCreateBuff(DxWrap, vert);

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
	void DxIndexProp::CopyBuffer() {
		unsigned short* mappedIdx = nullptr;

		mIndex_Buff->Map(0, nullptr, (void**)&mappedIdx);
		std::copy(
			std::begin(index),
			std::end(index),
			mappedIdx
		);
		mIndex_Buff->Unmap(0, nullptr);

	}
	D3D12_INDEX_BUFFER_VIEW
		DxIndexProp::getIndexView() {
		return mIndexView;
	}
}