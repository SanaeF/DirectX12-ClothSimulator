#pragma once
#include "../../Vertex/DxGraph2DVertex.h"

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxIndex2D :public Vertex2D {
	private:
		unsigned short index[6] = {
			0,1,2,
			2,1,3
		};

		ID3D12Resource* mIndex_Buff = nullptr;

		D3D12_INDEX_BUFFER_VIEW mIndexView = {};

	public:

		void CreateIndexBufferView(lib::DirectX12Manager& DxWrap);

		D3D12_INDEX_BUFFER_VIEW getIndexView();

	private:

		void CreateBuffer(lib::DirectX12Manager& DxWrap);

		void CopyBuffer();

	};
}