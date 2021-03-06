#include "DxGraph2DVertex.h"
#include "../../DirectX12/DirectX12Manager.h"

namespace libGraph {
	void Vertex2D::setPolygonSize(SIZE size, DirectX::TexMetadata data) {
		vertex[0].pos.x = -1;
		vertex[0].pos.y = -1;
		vertex[1].pos.x = -1;
		vertex[1].pos.y = 1;
		vertex[2].pos.x = 1;
		vertex[2].pos.y = -1;
		vertex[3].pos.x = 1;
		vertex[3].pos.y = 1;

		float widthProp = (static_cast<float>(data.width) / static_cast<float>(size.cx));
		float heightProp = (static_cast<float>(data.height) / static_cast<float>(size.cy));
		float aspect = static_cast<float>(size.cx) / static_cast<float>(size.cy);
		for (int i = 0; i < 4; i++) {
			vertex[i].pos.x = vertex[i].pos.x * widthProp * aspect;
			vertex[i].pos.y = vertex[i].pos.y * heightProp;
		}
	}

	void Vertex2D::changePol(float x, float y) {
		float posX = x / static_cast <float>(1920);
		float posY = y / static_cast <float>(1440);
		for (int i = 0; i < 4; i++) {
			vertex[i].pos.x = vertex[i].pos.x + posX;
			vertex[i].pos.y = vertex[i].pos.y + posY;
		}
	}

	void Vertex2D::Heap_Prop() {
		mHeap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		mHeap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		mHeap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	}


	void Vertex2D::mResourceDesc() {
		mResource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		mResource_desc.Width = sizeof(vertex);
		mResource_desc.Height = 1;
		mResource_desc.DepthOrArraySize = 1;
		mResource_desc.MipLevels = 1;
		mResource_desc.Format = DXGI_FORMAT_UNKNOWN;
		mResource_desc.SampleDesc.Count = 1;
		mResource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		mResource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}


	void Vertex2D::mBufferCCR(lib::DirectX12Manager& DxWrap) {
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


	void Vertex2D::mCopyBuffer() {
		auto result = mVertex_Buff->Map(0, nullptr, (void**)&mVertexMap);
		if (!SUCCEEDED(result)) {
			assert(0 && "2D Buffer Map Error!");
		}
		std::copy(
			std::begin(vertex),
			std::end(vertex),
			mVertexMap
		);

		mVertex_Buff->Unmap(0, nullptr);
	}


	void Vertex2D::mCretateBuffView() {
		verBuffView.BufferLocation = mVertex_Buff->GetGPUVirtualAddress();
		verBuffView.SizeInBytes = sizeof(vertex);
		verBuffView.StrideInBytes = sizeof(vertex[0]);
	}



	void Vertex2D::mCreateBuff(lib::DirectX12Manager& DxWrap) {
		mResourceDesc();

		mBufferCCR(DxWrap);

		mCopyBuffer();

		mCretateBuffView();
	}

	void Vertex2D::setResourceDesc(D3D12_RESOURCE_DESC Resource_desc) {
		mResource_desc.Width = Resource_desc.Width;
	}

	D3D12_VERTEX_BUFFER_VIEW
		Vertex2D::getvertexBufferView() {
		return verBuffView;
	}

	D3D12_HEAP_PROPERTIES
		Vertex2D::getHeap_prop() {
		return mHeap_prop;
	}

	D3D12_RESOURCE_DESC
		Vertex2D::getResource_desc() {
		return mResource_desc;
	}

}