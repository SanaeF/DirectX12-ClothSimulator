#include "DxGraphicsViewPort.h"	
#include"../../DirectX12Manager/DirectX12Manager.h"

namespace libGraph {
	DxViewPort2D::DxViewPort2D(SIZE size) {
		createViewPort(size);
	}

	void DxViewPort2D::createViewPort(SIZE size) {
		m_View_port.Width = size.cx;
		m_View_port.Height = size.cy;
		m_View_port.TopLeftX = 0;
		m_View_port.TopLeftY = 0;
		m_View_port.MaxDepth = 1.0f;
		m_View_port.MinDepth = 0.0f;
	}

	void DxViewPort2D::scissor(std::shared_ptr<lib::DirectX12Manager>& DxWrap) {
		m_Scissor_rect.top = 0;
		m_Scissor_rect.left = 0;
		m_Scissor_rect.right = m_Scissor_rect.left + DxWrap->getPixelSize().cx;
		m_Scissor_rect.bottom = m_Scissor_rect.top + DxWrap->getPixelSize().cy;
	}

	void DxViewPort2D::scissor(int top, int left, int right, int bottom) {
		m_Scissor_rect.top = top;
		m_Scissor_rect.left = left;
		m_Scissor_rect.right = right;
		m_Scissor_rect.bottom = bottom;
	}

	D3D12_VIEWPORT
		DxViewPort2D::getViewPort() {
		return m_View_port;
	}

	D3D12_RECT
		DxViewPort2D::getScissorrect() {
		return m_Scissor_rect;
	}
}