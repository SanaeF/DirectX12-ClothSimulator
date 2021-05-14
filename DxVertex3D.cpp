#include "DxVertex3D.h"

#include"Dx12Wrapper.h"

void DxVertex3D::LoadVertex(
	std::shared_ptr<Dx12Wrapper> DxWrap,
	FILE* fp
) {
	constexpr size_t pmdVertex_size = 38;

	fread(&vertNum, sizeof(vertNum), 1, fp);
	std::vector<unsigned char>vertex(vertNum * pmdVertex_size);
	fread(vertex.data(), vertex.size(), 1, fp);

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(vertex.size());
	DxWrap->Device()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&VertexBuff)
	);

	_vbView.BufferLocation = VertexBuff->GetGPUVirtualAddress();//バッファの仮想アドレス
	_vbView.SizeInBytes = vertex.size();//全バイト数
	_vbView.StrideInBytes = pmdVertex_size;//1頂点あたりのバイト数

	unsigned char* vertexMap = nullptr;

	auto result = VertexBuff->Map(0, nullptr, (void**)&vertexMap);
}

ID3D12Resource* 
DxVertex3D::getVertexBuff() {
	return VertexBuff;
}
D3D12_VERTEX_BUFFER_VIEW 
DxVertex3D::getVertexBuffView() {
	return _vbView;
}
D3D12_INDEX_BUFFER_VIEW 
DxVertex3D::getIndexBuffView() {
	return _ibView;
}