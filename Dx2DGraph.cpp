#include "Dx2DGraph.h"
#include"Dx12Wrapper.h"

#include "DxTexture2D.h"
#include "Dx2DMatrix.h"
#include "DxIndex2D.h"
#include "Dx2DRootSignature.h"
#include "Dx2DPipeline.h"

int Dx2DGraph::mHandleCount = 0;

int Dx2DGraph::Load(const wchar_t* path) {
	int HandleID = mHandleCount;
	mMatrix->createBuffer(*mDxWrap,HandleID);

	mTexture->LoadWIC(path);
	mTexture->CreateResource(mDxWrap);
	mTexture->ShaderResourceView(mDxWrap, mMatrix->getConstBuffer(HandleID));
	mTexture->GPUtransfer();
	GraphicPipeline();

	SetHandleData(HandleID);

	return HandleID;
}

void Dx2DGraph::Draw(double Angle, int Handle) {
	if (Handle == -1 || Handle >= mHandleCount)return;
	mMatrix->Rotation(0, 0, Angle, Handle);
	mDxWrap->CommandList()->SetPipelineState(mGraphData[Handle].mPipelineState);
	SetRS(Handle);
	mDxWrap->CommandList()->SetGraphicsRootSignature(mGraphData[Handle].mRootSignature);
	SetGraphicsRTB(Handle);
	mDxWrap->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SetBuffer(Handle);
	mDxWrap->CommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


void Dx2DGraph::SetRS(int Handle) {
	mDxWrap->CommandList()->RSSetViewports(1, mDxWrap->getViewPort());
	mDxWrap->CommandList()->RSSetScissorRects(1, mDxWrap->getScissorrect());
}

void Dx2DGraph::SetBuffer(int Handle) {
	mDxWrap->CommandList()->IASetVertexBuffers(0, 1, &mGraphData[Handle].mVertexBufferView);
	mDxWrap->CommandList()->IASetIndexBuffer(&mGraphData[Handle].mVertexIndexView);
}

void Dx2DGraph::SetGraphicsRTB(int Handle) {
	mDxWrap->CommandList()->SetDescriptorHeaps(1, &mGraphData[Handle].mTexDescHeap);
	mDxWrap->CommandList()->SetGraphicsRootDescriptorTable(
		0,
		mGraphData[Handle].mTexDescHeap->GetGPUDescriptorHandleForHeapStart()
	);

	auto heapHandle = mGraphData[Handle].mTexDescHeap->GetGPUDescriptorHandleForHeapStart();

	heapHandle.ptr += mDxWrap->Device()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	);
	mDxWrap->CommandList()->SetGraphicsRootDescriptorTable(1, heapHandle);
}


void Dx2DGraph::GraphicPipeline() {
	mPipeline->LoadShader();
	mPipeline->ShaderState();
	mPipeline->SampleMaskState();
	mPipeline->RasterizerState();
	mPipeline->BlendState();
	mPipeline->Layout();
	mPipeline->RenderTargets();
	mPipeline->AntiAiliasing();
	mPipeline->RootSignatureState(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	mPipeline->CreateGraphicsPipeline(mDxWrap);
}

void Dx2DGraph::SetHandleData(int Handle) {
	mGraphData[Handle].mTexDescHeap = mTexture->getBasicDescHeap();
	mGraphData[Handle].mPipelineState = mPipeline->getPipelineState();
	mGraphData[Handle].mRootSignature = mRootSignature->getRootSignature();
	mGraphData[Handle].mVertexBufferView = mIndex->getvertexBufferView();
	mGraphData[Handle].mVertexIndexView = mIndex->getIndexView();

	mHandleCount++;
}

Dx2DGraph::Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap) :mDxWrap(DxWrap) {
	mTexture.reset(new DxTexture2D());
	mMatrix.reset(new Dx2DMatrix());
	mIndex.reset(new DxIndex2D());
	mPipeline.reset(new Dx2DPipeline());
	mRootSignature.reset(new Dx2DRootSignature());
	mIndex->CreateIndexBufferView(*DxWrap);
}

Dx2DGraph::~Dx2DGraph() {

}