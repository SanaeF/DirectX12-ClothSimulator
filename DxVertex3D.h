#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>

class Dx12Wrapper;
class DxVertex3D {
private:
	//ハンドルに入れるべきデータ
	unsigned int vertNum;

	ID3D12Resource* VertexBuff;

	D3D12_VERTEX_BUFFER_VIEW _vbView = {};
	D3D12_INDEX_BUFFER_VIEW _ibView = {};

public:

	void LoadVertex(
		std::shared_ptr<Dx12Wrapper> DxWrap,
		FILE* fp
	);

	ID3D12Resource* getVertexBuff();

	D3D12_VERTEX_BUFFER_VIEW getVertexBuffView();
	D3D12_INDEX_BUFFER_VIEW getIndexBuffView();
};