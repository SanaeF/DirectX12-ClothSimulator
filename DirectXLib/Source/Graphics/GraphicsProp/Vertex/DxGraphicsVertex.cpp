#include "DxGraphicsVertex.h"
#include "../../../DirectX12/DirectX12Manager.h"

namespace libGraph {
	void DxVertexProp::set2DPolygonSize(Vertex vert[4], SIZE size, DirectX::TexMetadata data) {
		vert[0].pos.x = -1;
		vert[0].pos.y = -1;
		vert[1].pos.x = -1;
		vert[1].pos.y = 1;
		vert[2].pos.x = 1;
		vert[2].pos.y = -1;
		vert[3].pos.x = 1;
		vert[3].pos.y = 1;

		float widthProp = (static_cast<float>(data.width) / static_cast<float>(size.cx));
		float heightProp = (static_cast<float>(data.height) / static_cast<float>(size.cy));
		float aspect = static_cast<float>(size.cx) / static_cast<float>(size.cy);
		for (int i = 0; i < 4; i++) {
			vert[i].pos.x = vert[i].pos.x * widthProp * aspect;
			vert[i].pos.y = vert[i].pos.y * heightProp;
		}
	}
	void DxVertexProp::changePol(Vertex vert[4], float x, float y) {
		float posX = x / static_cast <float>(1920);
		float posY = y / static_cast <float>(1440);
		for (int i = 0; i < 4; i++) {
			vert[i].pos.x = vert[i].pos.x + posX;
			vert[i].pos.y = vert[i].pos.y + posY;
		}
	}
	void DxVertexProp::Heap_Prop() {
		mHeap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		mHeap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		mHeap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	}

	void DxVertexProp::mCreateBuff(lib::DirectX12Manager& DxWrap, std::vector<Vertex> vert) {
		mResourceDesc(vert);

		mBufferCCR(DxWrap);

		mCopyBuffer(vert);

		mCretateBuffView(vert);
	}
	void DxVertexProp::mResourceDesc(std::vector<Vertex> vert) {
		mResource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		mResource_desc.Width = sizeof(vert);
		mResource_desc.Height = 1;
		mResource_desc.DepthOrArraySize = 1;
		mResource_desc.MipLevels = 1;
		mResource_desc.Format = DXGI_FORMAT_UNKNOWN;
		mResource_desc.SampleDesc.Count = 1;
		mResource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		mResource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}
	void DxVertexProp::mBufferCCR(lib::DirectX12Manager& DxWrap) {
		auto result = DxWrap.Device()->CreateCommittedResource(
			&mHeap_prop,
			D3D12_HEAP_FLAG_NONE,
			&mResource_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertex_Buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "2D CreateBuff...ResourceComitt Error!");
		}
	}
	void DxVertexProp::mCopyBuffer(std::vector<Vertex> vert) {
		auto result = mVertex_Buff->Map(0, nullptr, (void**)&mVertexMap);
		if (!SUCCEEDED(result)) {
			assert(0 && "2D Buffer Map Error!");
		}
		std::copy(
			vert.begin(),
			vert.end(),
			mVertexMap
		);

		mVertex_Buff->Unmap(0, nullptr);
	}
	void DxVertexProp::mCretateBuffView(std::vector<Vertex> vert) {
		verBuffView.BufferLocation = mVertex_Buff->GetGPUVirtualAddress();
		verBuffView.SizeInBytes = sizeof(vert);
		verBuffView.StrideInBytes = sizeof(vert[0]);
	}

	void DxVertexProp::setResourceDesc(D3D12_RESOURCE_DESC Resource_desc) {
		mResource_desc.Width = Resource_desc.Width;
	}
	D3D12_VERTEX_BUFFER_VIEW DxVertexProp::getvertexBufferView() {
		return verBuffView;
	}
	D3D12_HEAP_PROPERTIES DxVertexProp::getHeap_prop() {
		return mHeap_prop;
	}
	D3D12_RESOURCE_DESC DxVertexProp::getResource_desc() {
		return mResource_desc;
	}
}