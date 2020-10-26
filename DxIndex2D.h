#pragma once
#include "Vertex2D.h"

class Dx12Wrapper;

class DxIndex2D :public Vertex2D {
private:
	unsigned short index[6] = {
		0,1,2,
		2,1,3
	};

	ID3D12Resource* mIndex_Buff = nullptr;

	D3D12_INDEX_BUFFER_VIEW mIndexView = {};

public:

	void CreateIndexBufferView(Dx12Wrapper& DxWrap);

	D3D12_INDEX_BUFFER_VIEW getIndexView();

private:

	void CreateBuffer(Dx12Wrapper& DxWrap);

	void CopyBuffer();

};