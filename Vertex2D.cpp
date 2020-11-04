#include "Vertex2D.h"
#include "Dx12Wrapper.h"


void Vertex2D::setPolygonSize(DirectX::TexMetadata data) {
	float aspect = static_cast<float>(data.width) / static_cast<float>(data.height);
	//vertex[0].pos.y = vertex[0].pos.x / aspect;
	//vertex[2].pos.y = vertex[0].pos.x / aspect;
	vertex[0].pos.x = vertex[0].pos.y * aspect;
	vertex[1].pos.x = vertex[0].pos.y * aspect;
	vertex[2].pos.x = vertex[3].pos.y * aspect;
	vertex[3].pos.x = vertex[3].pos.y * aspect;
	//vertex[0].pos.y = data.height;
	//vertex[2].pos.x = data.width;
	//vertex[2].pos.y = data.height;

	//vertex[3].pos.x = data.width;
}

void Vertex2D::mHeapProp() {
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


void Vertex2D::mBufferCCR(Dx12Wrapper& DxWrap) {
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



void Vertex2D::mCreateBuff(Dx12Wrapper& DxWrap) {
	mHeapProp();

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

