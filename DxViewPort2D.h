#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>
//#include<memory>
class Dx12Wrapper;

class DxViewPort2D {
private:
	D3D12_VIEWPORT mViewPort = {};
	D3D12_RECT mScissorrect = {};
public:

	void CreateViewPort(SIZE size, DirectX::TexMetadata MetaData);

	void Scissor(std::shared_ptr<Dx12Wrapper> DxWrap);

	void Scissor(int top, int left, int right, int bottom);

	D3D12_VIEWPORT getViewPort();

	D3D12_RECT getScissorrect();

};