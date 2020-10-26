#include "DXGraph.h"

#include"Dx12Wrapper.h"
#include "Dx2DGraph.h"

DXGraph::DXGraph(std::shared_ptr<Dx12Wrapper> DxWrap)
{
	mDraw2D.reset(new Dx2DGraph(DxWrap));
}

DXGraph::~DXGraph() {

}

int DXGraph::Load2D(const wchar_t* path) {
	return mDraw2D->Load(path);
}

void DXGraph::DrawPrototype2D(double Angle, int Handle) {
	mDraw2D->Draw(Angle, Handle);
}

void DXGraph::Init() {

}