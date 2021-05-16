#include "DxGraphics.h"

#include "../DirectX12/DirectX12Manager.h"
#include "../Graphics/Graph2D/DxGraphics2D.h"
#include "../Graphics/Graph3D/DxGraphics3D.h"
#include "../../Source/Graphics/GraphPipline/DxGraphicsPipeline.h"
#include "../../../DirectXLib/Source/Graphics/ViewPort/DxGraphicsViewPort.h"

namespace lib {
	DxGraph::DxGraph(std::shared_ptr<DirectX12Manager>& DxWrap) :
		mDxWrap(DxWrap)
	{
		mDxWrap->ClearScreen();
		mDraw2D.reset(new Dx2DGraph(DxWrap));
		mDraw3D.reset(new Graph3D(DxWrap));
		mPipeline2D.reset(new libGraph::Dx2DPipeline(libGraph::Dx2DPipeline::Shader2D));
		mPipeline3D.reset(new libGraph::Dx2DPipeline(libGraph::Dx2DPipeline::Shader3D));
		mViewPort.reset(new libGraph::DxViewPort2D(mDxWrap->getPixelSize()));
		auto view = mViewPort->getViewPort();
		mDxWrap->CmdList()->RSSetViewports(1, &view);
	}

	DxGraph::~DxGraph() {

	}

	void DxGraph::SetArea(int x1, int y1, int x2, int y2) {
		mViewPort->Scissor(x1, y1, x2, y2);
		auto getScissor = mViewPort->getScissorrect();
		mDxWrap->CmdList()->RSSetScissorRects(1, &getScissor);
	}

	int DxGraph::Load2D(const wchar_t* path) {
		return mDraw2D->Load(path, *mPipeline2D);
	}

	void DxGraph::DrawPrototype2D(float x, float y, float size, float Angle, int Handle) {
		mDraw2D->BeingDraw(*mPipeline2D);
		mDraw2D->Draw(x, y, size, Angle, Handle);
	}


	int DxGraph::Load3D() {
		mDraw3D->loadFbx(*mPipeline3D);
		return 0;
	}

	void DxGraph::Draw3D(float x, float y, float size, float Angle) {
		mDraw3D->beginDraw(*mPipeline3D, 0);
		mDraw3D->Draw(x, y, size, Angle, 0);
		//mDraw2D->BeingDraw();
	}
	void DxGraph::clothSimProc(int Handle) {
		mDraw3D->ClothSimProc(Handle);
	}
	void DxGraph::ClearDraw() {
		mDxWrap->ClearScreen();
		auto viewPort = mViewPort->getViewPort();
		mDxWrap->CmdList()->RSSetViewports(1, &viewPort);
		
		
	}

	void DxGraph::ScreenFlip() {
		mDxWrap->ScreenFlip();
	}

	void DxGraph::Init() {

	}
}