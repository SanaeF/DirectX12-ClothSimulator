#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>
namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxViewPort2D {
	private:
		D3D12_VIEWPORT m_View_port = {};
		D3D12_RECT m_Scissor_rect = {};
	public:
		DxViewPort2D(SIZE size);
		void createViewPort(SIZE size);
		void scissor(std::shared_ptr<lib::DirectX12Manager>& DxWrap);
		void scissor(int top, int left, int right, int bottom);

		D3D12_VIEWPORT getViewPort();
		D3D12_RECT getScissorrect();
	};
}