#include "DxViewPort2D.h"	

#include"Dx12Wrapper.h"

void DxViewPort2D::CreateViewPort(SIZE size, DirectX::TexMetadata MetaData) {
	mViewPort.Width = size.cx;
	mViewPort.Height = size.cy;
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.MinDepth = 0.0f;
}

void DxViewPort2D::Scissor(std::shared_ptr<Dx12Wrapper> DxWrap) {
	mScissorrect.top = 0;
	mScissorrect.left = 0;
	mScissorrect.right = mScissorrect.left + DxWrap->getPixelSize().cx;
	mScissorrect.bottom = mScissorrect.top + DxWrap->getPixelSize().cy;
}

void DxViewPort2D::Scissor(int top, int left, int right, int bottom) {
	mScissorrect.top = top;
	mScissorrect.left = left;
	mScissorrect.right = right;
	mScissorrect.bottom = bottom;
}

D3D12_VIEWPORT 
DxViewPort2D::getViewPort() {
	return mViewPort;
}

D3D12_RECT 
DxViewPort2D::getScissorrect() {
	return mScissorrect;
}