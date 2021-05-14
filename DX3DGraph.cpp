#include "DX3DGraph.h"

#include"Dx12Wrapper.h"

int Dx3DGraph::Load() {
	return 0;
}

Dx3DGraph::Dx3DGraph(std::shared_ptr<Dx12Wrapper> DxWrap):mDxWrap(DxWrap){
}
Dx3DGraph::~Dx3DGraph() {
}