#include "ModelData.h"

namespace lib {
	ModelData ModelData::Object;
	ModelData::ModelData() {
	}
	void ModelData::createViewBuffer(ComPtr<ID3D12Device> device) {
		if (!createVertexBuffer(device))return;
		if (!createIndexBuffer(device))return;
	}
	bool ModelData::createVertexBuffer(ComPtr<ID3D12Device> device) {
		const UINT vertexBufferSize = static_cast<UINT>(vertex.size()) * sizeof(lib::ModelParam);
		constexpr size_t vertexSize = 38;
		ID3D12Resource* vb = nullptr;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		auto result = device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&vb));//ファイルが無いとエラー
		UINT8* pVertexDataBegin;
		m_Read_range.Begin = 0;
		m_Read_range.End = 0;
		vb->Map(
			0, &m_Read_range, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, vertex.data(), vertexBufferSize);
		vb->Unmap(0, nullptr);
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		vertex_buffer_view.BufferLocation = vb->GetGPUVirtualAddress();
		vertex_buffer_view.SizeInBytes = vertexBufferSize;
		vertex_buffer_view.StrideInBytes = sizeof(lib::ModelParam);
		this->vertex_buffer = vb;
		vb_view = vertex_buffer_view;
		return true;
	}
	bool ModelData::createIndexBuffer(ComPtr<ID3D12Device> device) {
		//helper::LibHelper help;
		const UINT indexBufferSize = static_cast<UINT>(index.size()) * sizeof(UINT);
		ID3D12Resource* ib = nullptr;
		CD3DX12_HEAP_PROPERTIES heapProperty =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferResource =
			CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		device->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&ib));
		UINT8* pIndexDataBegin;
		ib->Map(
			0, &m_Read_range, reinterpret_cast<void**>(&pIndexDataBegin));
		memcpy(pIndexDataBegin, index.data(), indexBufferSize);
		ib->Unmap(0, nullptr);
		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		index_buffer_view.BufferLocation = ib->GetGPUVirtualAddress();
		index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		index_buffer_view.SizeInBytes = indexBufferSize;
		index_buffer = ib;
		ib_view = index_buffer_view;
		return true;
	}
}