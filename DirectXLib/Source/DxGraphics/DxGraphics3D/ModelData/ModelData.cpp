#include "ModelData.h"

namespace lib {
	ModelData ModelData::Object;
	ModelData::ModelData(){
	}
	void ModelData::initialize() {
		vertex.clear();
		param.clear();
		index.clear();
		index_group.clear();
		texturePaths.clear();
		materials.clear();
		bones.clear();
		vertex_buffer = nullptr;
		index_buffer = nullptr;
		mat_heap = nullptr;
		m_Vb = nullptr;
		m_Ib = nullptr;
		m_Vert_size = 0;
		m_Idx_size = 0;
	}
	void ModelData::createViewBuffer(ComPtr<ID3D12Device> device) {
		if (!createVertexBuffer(device))return;
		if (!createIndexBuffer(device))return;
		bufferMap();
	}
	void ModelData::bufferMap() {
		const UINT vertexBufferSize = static_cast<UINT>(vertex.size()) * sizeof(lib::ModelVertex);
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE range{ 0,0 };
		m_Vb->Map(
			0, &range, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, vertex.data(), vertexBufferSize);
		m_Vb->Unmap(0, nullptr);
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		vertex_buffer_view.BufferLocation = m_Vb->GetGPUVirtualAddress();
		vertex_buffer_view.SizeInBytes = vertexBufferSize;
		vertex_buffer_view.StrideInBytes = sizeof(lib::ModelVertex);
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
		m_Vert_size = static_cast<UINT>(vertex.size()) * sizeof(lib::ModelVertex);
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
	void ModelData::createMaterialBuffer(ComPtr<ID3D12Device> device) {
		//マテリアルバッファを作成
		auto materialBuffSize = sizeof(Material);
		materialBuffSize = (materialBuffSize + 0xff) & ~0xff;
		ID3D12Resource* materialBuff = nullptr;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(materialBuffSize * materials.size());
		auto result = device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&materialBuff)
		);

		//マップマテリアルにコピー
		char* mapMaterial = nullptr;
		result = materialBuff->Map(0, nullptr, (void**)&mapMaterial);
		for (auto& m : materials) {
			*((Material*)mapMaterial) = m;//データコピー
			mapMaterial += materialBuffSize;//次のアライメント位置まで進める
		}
		materialBuff->Unmap(0, nullptr);


		D3D12_DESCRIPTOR_HEAP_DESC materialDescHeapDesc = {};
		materialDescHeapDesc.NumDescriptors = materials.size() * 5;//マテリアル数ぶん(定数1つ、テクスチャ3つ)
		materialDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		materialDescHeapDesc.NodeMask = 0;

		materialDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別
		result = device->CreateDescriptorHeap(&materialDescHeapDesc, IID_PPV_ARGS(&mat_heap));//生成

		D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
		matCBVDesc.BufferLocation = materialBuff->GetGPUVirtualAddress();
		matCBVDesc.SizeInBytes = materialBuffSize;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1

		auto matDescHeapH = mat_heap->GetCPUDescriptorHandleForHeapStart();
		auto incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		for (int i = 0; i < materials.size(); ++i) {
			//マテリアル固定バッファビュー
			device->CreateConstantBufferView(&matCBVDesc, matDescHeapH);
			matDescHeapH.ptr += incSize;
			matCBVDesc.BufferLocation += materialBuffSize;
		}
	}
}