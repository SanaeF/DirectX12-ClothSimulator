#include "DxGraph2DIndex.h"
#include "../../../../DirectX12Manager/DirectX12Manager.h"

namespace libGraph {
	void DxGraph2DIndex::createBuffer(lib::DirectX12Manager& DxWrap) {
		createBuff(DxWrap);

		D3D12_RESOURCE_DESC mResource_desc = {};
		mResource_desc.Width = sizeof(m_Index);
		setResourceDesc(mResource_desc);
		auto heapProp = getHeapProp();
		auto resDesc = getResourceDesc();
		auto result = DxWrap.device()->CreateCommittedResource(//
			&heapProp,//
			D3D12_HEAP_FLAG_NONE,//
			&resDesc,//
			D3D12_RESOURCE_STATE_GENERIC_READ,//
			nullptr,
			IID_PPV_ARGS(&m_pIndex_buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "CreateCommittedResource");
		}

	}

	void DxGraph2DIndex::copyBuffer() {
		unsigned short* mappedIdx = nullptr;

		m_pIndex_buff->Map(0, nullptr, (void**)&mappedIdx);
		std::copy(
			std::begin(m_Index),
			std::end(m_Index),
			mappedIdx
		);
		m_pIndex_buff->Unmap(0, nullptr);

	}

	void DxGraph2DIndex::createIndexBufferView(lib::DirectX12Manager& DxWrap) {
		createBuffer(DxWrap);
		copyBuffer();
		m_Index_view.BufferLocation = m_pIndex_buff->GetGPUVirtualAddress();
		m_Index_view.Format = DXGI_FORMAT_R16_UINT;
		m_Index_view.SizeInBytes = sizeof(m_Index);
	}

	D3D12_INDEX_BUFFER_VIEW
		DxGraph2DIndex::getIndexView() {
		return m_Index_view;
	}
}