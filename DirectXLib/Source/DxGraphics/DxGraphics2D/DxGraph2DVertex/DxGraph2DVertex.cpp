#include "DxGraph2DVertex.h"
#include "../../../DirectX12Manager/DirectX12Manager.h"

namespace libGraph {
	void DxGraph2DVertex::setPolygonSize(SIZE size, DirectX::TexMetadata data) {
		m_Vertex[0].pos.x = -1;
		m_Vertex[0].pos.y = -1;
		m_Vertex[1].pos.x = -1;
		m_Vertex[1].pos.y = 1;
		m_Vertex[2].pos.x = 1;
		m_Vertex[2].pos.y = -1;
		m_Vertex[3].pos.x = 1;
		m_Vertex[3].pos.y = 1;

		float widthProp = (static_cast<float>(data.width) / static_cast<float>(size.cx));
		float heightProp = (static_cast<float>(data.height) / static_cast<float>(size.cy));
		float aspect = static_cast<float>(size.cx) / static_cast<float>(size.cy);
		for (int i = 0; i < 4; i++) {
			m_Vertex[i].pos.x = m_Vertex[i].pos.x * widthProp * aspect;
			m_Vertex[i].pos.y = m_Vertex[i].pos.y * heightProp;
		}
	}

	void DxGraph2DVertex::changePolygon(float x, float y) {
		float posX = x / static_cast <float>(1920);
		float posY = y / static_cast <float>(1440);
		for (int i = 0; i < 4; i++) {
			m_Vertex[i].pos.x = m_Vertex[i].pos.x + posX;
			m_Vertex[i].pos.y = m_Vertex[i].pos.y + posY;
		}
	}

	void DxGraph2DVertex::heapProp() {
		m_Heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		m_Heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		m_Heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	}


	void DxGraph2DVertex::resourceDesc() {
		m_Resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		m_Resource_desc.Width = sizeof(m_Vertex);
		m_Resource_desc.Height = 1;
		m_Resource_desc.DepthOrArraySize = 1;
		m_Resource_desc.MipLevels = 1;
		m_Resource_desc.Format = DXGI_FORMAT_UNKNOWN;
		m_Resource_desc.SampleDesc.Count = 1;
		m_Resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		m_Resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}


	void DxGraph2DVertex::commitBuffer(lib::DirectX12Manager& dx_12) {
		auto result = dx_12.device()->CreateCommittedResource(
			&m_Heap_prop,
			D3D12_HEAP_FLAG_NONE,
			&m_Resource_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_Vertex_buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "2D CreateBuff...ResourceComitt Error!");
		}
	}


	void DxGraph2DVertex::copyBuffer() {
		auto result = m_Vertex_buff->Map(0, nullptr, (void**)&m_pVertex_map);
		if (!SUCCEEDED(result)) {
			assert(0 && "2D Buffer Map Error!");
		}
		std::copy(
			std::begin(m_Vertex),
			std::end(m_Vertex),
			m_pVertex_map
		);

		m_Vertex_buff->Unmap(0, nullptr);
	}


	void DxGraph2DVertex::cretateBuffView() {
		m_VB_view.BufferLocation = m_Vertex_buff->GetGPUVirtualAddress();
		m_VB_view.SizeInBytes = sizeof(m_Vertex);
		m_VB_view.StrideInBytes = sizeof(m_Vertex[0]);
	}



	void DxGraph2DVertex::createBuff(lib::DirectX12Manager& dx_12) {
		resourceDesc();

		commitBuffer(dx_12);

		copyBuffer();

		cretateBuffView();
	}

	void DxGraph2DVertex::setResourceDesc(D3D12_RESOURCE_DESC Resource_desc) {
		m_Resource_desc.Width = Resource_desc.Width;
	}

	D3D12_VERTEX_BUFFER_VIEW
		DxGraph2DVertex::getVertexBufferView() {
		return m_VB_view;
	}

	D3D12_HEAP_PROPERTIES
		DxGraph2DVertex::getHeapProp() {
		return m_Heap_prop;
	}

	D3D12_RESOURCE_DESC
		DxGraph2DVertex::getResourceDesc() {
		return m_Resource_desc;
	}

}