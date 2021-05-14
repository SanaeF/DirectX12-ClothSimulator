#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>
//#include<memory>
namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxViewPort2D {
	private:
		D3D12_VIEWPORT mViewPort = {};
		D3D12_RECT mScissorrect = {};
	public:
		DxViewPort2D(SIZE size);

		void CreateViewPort(SIZE size);

		void Scissor(std::shared_ptr<lib::DirectX12Manager>& DxWrap);

		void Scissor(int top, int left, int right, int bottom);

		D3D12_VIEWPORT getViewPort();

		D3D12_RECT getScissorrect();

	};
}