#include "ModelData.h"

namespace lib {
	ModelData ModelData::Object;
	ModelData::ModelData(){
	}
	void ModelData::createViewBuffer(ComPtr<ID3D12Device> device) {
		if (!createVertexBuffer(device))return;
		if (!createIndexBuffer(device))return;
		bufferMap();
	}
	void ModelData::bufferMap() {
		const UINT vertexBufferSize = static_cast<UINT>(vertex.size()) * sizeof(lib::ModelParam);
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE range{ 0,0 };
		m_Vb->Map(
			0, &range, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, vertex.data(), vertexBufferSize);
		m_Vb->Unmap(0, nullptr);
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		vertex_buffer_view.BufferLocation = m_Vb->GetGPUVirtualAddress();
		vertex_buffer_view.SizeInBytes = vertexBufferSize;
		vertex_buffer_view.StrideInBytes = sizeof(lib::ModelParam);
		this->vertex_buffer = m_Vb;
		vb_view = vertex_buffer_view;

		const UINT indexBufferSize = static_cast<UINT>(index.size()) * sizeof(UINT);
		UINT8* pIndexDataBegin;
		range = { 0,0 };
		m_Ib->Map(
			0, &range, reinterpret_cast<void**>(&pIndexDataBegin));
		memcpy(pIndexDataBegin, index.data(), indexBufferSize);
		m_Ib->Unmap(0, nullptr);
		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		index_buffer_view.BufferLocation = m_Ib->GetGPUVirtualAddress();
		index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		index_buffer_view.SizeInBytes = indexBufferSize;
		index_buffer = m_Ib;
		ib_view = index_buffer_view;
	}
	bool ModelData::createVertexBuffer(ComPtr<ID3D12Device> device) {
		m_Vert_size = static_cast<UINT>(vertex.size()) * sizeof(lib::ModelParam);
		constexpr size_t vertexSize = 38;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(m_Vert_size);
		auto result = device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Vb));//ファイルが無いとエラー
		return true;
	}
	bool ModelData::createIndexBuffer(ComPtr<ID3D12Device> device) {
		m_Idx_size = static_cast<UINT>(index.size()) * sizeof(UINT);
		CD3DX12_HEAP_PROPERTIES heapProperty =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferResource =
			CD3DX12_RESOURCE_DESC::Buffer(m_Idx_size);
		device->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Ib));
		return true;
	}
}