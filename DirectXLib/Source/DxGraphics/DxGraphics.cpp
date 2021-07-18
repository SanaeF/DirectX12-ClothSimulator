#include "DxGraphics.h"

#include "../DirectX12Manager/DirectX12Manager.h"
#include "DxGraphics2D/DxGraphics2D.h"
#include "DxGraphics3D/DxGraphics3D.h"
#include "DxGraphicsPipeline/DxGraphicsPipeline.h"
#include "ViewPort/DxGraphicsViewPort.h"
namespace lib {
	DxGraphics::DxGraphics(std::shared_ptr<DirectX12Manager>& dx_12) :
		m_Dx12(dx_12)
	{
		m_Dx12->clearScreen();
		m_Graphics2D.reset(new DxGraphics2D(dx_12));
		m_Graphics3D.reset(new DxGraphics3D(dx_12));
		m_Pipeline2D.reset(new libGraph::DxGraphicsPipeline(libGraph::DxGraphicsPipeline::SHADER_2D));
		m_Pipeline3D.reset(new libGraph::DxGraphicsPipeline(libGraph::DxGraphicsPipeline::SHADER_3D));
		m_View_port.reset(new libGraph::DxViewPort2D(m_Dx12->getPixelSize()));
		auto view = m_View_port->getViewPort();
		m_Dx12->cmdList()->RSSetViewports(1, &view);
	}

	DxGraphics::~DxGraphics() {

	}

	void DxGraphics::setArea(int x1, int y1, int x2, int y2) {
		m_View_port->scissor(x1, y1, x2, y2);
		auto getScissor = m_View_port->getScissorrect();
		m_Dx12->cmdList()->RSSetScissorRects(1, &getScissor);
	}

	int DxGraphics::load2D(const wchar_t* path) {
		return m_Graphics2D->load(path, *m_Pipeline2D);
	}

	void DxGraphics::drawPrototype2D(float x, float y, float size, float Angle, int Handle) {
		m_Graphics2D->beingDraw(*m_Pipeline2D);
		m_Graphics2D->draw(x, y, size, Angle, Handle);
	}


	int DxGraphics::load3D(const char* path) {
		m_Graphics3D->loadFbx(path, *m_Pipeline3D);
		return 0;
	}

	void DxGraphics::draw3D(float x, float y, float z, float size, float Angle) {
		m_Graphics3D->beginDraw(*m_Pipeline3D, 0);
		m_Graphics3D->draw(x, y, z, size, Angle, 0);
		//mDraw2D->BeingDraw();
	}
	void DxGraphics::setupClothSimulator(int Handle) {
		m_Graphics3D->setupClothPhis(Handle);
	}
	void DxGraphics::clothSimProc(int Handle) {
		m_Graphics3D->updateClothPhis(Handle);
	}
	void DxGraphics::clothReset(int Handle) {
		m_Graphics3D->resetClothPhis(Handle);
	}
	void DxGraphics::clearDraw() {
		m_Dx12->clearScreen();
		auto viewPort = m_View_port->getViewPort();
		m_Dx12->cmdList()->RSSetViewports(1, &viewPort);
		
		
	}

	void DxGraphics::screenFlip() {
		m_Dx12->screenFlip();
	}

	void DxGraphics::init() {

	}
}