#include "DxGraph.h"

#include"Dx12Wrapper.h"
#include "Dx2DGraph.h"

DxGraph::DxGraph(std::shared_ptr<Dx12Wrapper> DxWrap)
{
	mDraw2D.reset(new Dx2DGraph(DxWrap));
}

DxGraph::~DxGraph() {

}

void DxGraph::SetDrawArea(int top, int left, int right, int bottom) {
	mDraw2D->SetDrawArea(top, left, right, bottom);
}

int DxGraph::Load2D(const wchar_t* path) {
	return mDraw2D->Load(path);
}

void DxGraph::DrawPrototype2D(float x, float y, float size, float Angle, int Handle) {
	mDraw2D->Draw(x, y, size, Angle, Handle);
}

void DxGraph::Init() {

}
