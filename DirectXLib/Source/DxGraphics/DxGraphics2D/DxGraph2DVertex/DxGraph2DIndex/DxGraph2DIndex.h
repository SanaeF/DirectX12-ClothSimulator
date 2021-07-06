#pragma once
#include "../DxGraph2DVertex.h"

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxGraph2DIndex :public DxGraph2DVertex {
	private:
		unsigned short m_Index[6] = {
			0,1,2,
			2,1,3
		};
		ID3D12Resource* m_pIndex_buff = nullptr;
		D3D12_INDEX_BUFFER_VIEW m_Index_view = {};
	public:
		void createIndexBufferView(lib::DirectX12Manager& DxWrap);
		D3D12_INDEX_BUFFER_VIEW getIndexView();
	private:
		void createBuffer(lib::DirectX12Manager& DxWrap);
		void copyBuffer();
	};
}