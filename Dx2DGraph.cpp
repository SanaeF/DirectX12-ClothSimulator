#include "Dx2DGraph.h"
#include"Dx12Wrapper.h"

#include "DxTexture2D.h"

#include "DxIndex2D.h"
#include "Dx2DRootSignature.h"
#include "Dx2DPipeline.h"

int Dx2DGraph::mHandleCount = 0;

int Dx2DGraph::Load(const wchar_t* path) {
	int HandleID = mHandleCount;
	mTexture->LoadWIC(path);
	mTexture->CreateResource(mDxWrap);
	mTexture->ShaderResourceView(mDxWrap);
	mTexture->GPUtransfer();
	GraphicPipeline();

	mGraphData[HandleID].mTexDescHeap = mTexture->getTexDescHeap();
	mGraphData[HandleID].mPipelineState = mPipeline->getPipelineState();
	mGraphData[HandleID].mRootSignature = mRootSignature->getRootSignature();
	mGraphData[HandleID].mVertexBufferView = mIndex->getvertexBufferView();
	mGraphData[HandleID].mVertexIndexView = mIndex->getIndexView();

	mHandleCount++;
	return HandleID;
}

void Dx2DGraph::Draw(int Handle) {
	if (Handle == -1 || Handle >= mHandleCount)return;


	mDxWrap->CommandList()->SetPipelineState(mGraphData[Handle].mPipelineState);

	mDxWrap->CommandList()->RSSetViewports(1, mDxWrap->getViewPort());
	mDxWrap->CommandList()->RSSetScissorRects(1, mDxWrap->getScissorrect());

	mDxWrap->CommandList()->SetGraphicsRootSignature(mGraphData[Handle].mRootSignature);
	mDxWrap->CommandList()->SetDescriptorHeaps(1, &mGraphData[Handle].mTexDescHeap);
	mDxWrap->CommandList()->SetGraphicsRootDescriptorTable(
		0,
		mGraphData[Handle].mTexDescHeap->GetGPUDescriptorHandleForHeapStart()
	);

	mDxWrap->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDxWrap->CommandList()->IASetVertexBuffers(0, 1, &mGraphData[Handle].mVertexBufferView);
	mDxWrap->CommandList()->IASetIndexBuffer(&mGraphData[Handle].mVertexIndexView);
	
	mDxWrap->CommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
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

Dx2DGraph::Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap) :mDxWrap(DxWrap) {
	mTexture.reset(new DxTexture2D());
	mIndex.reset(new DxIndex2D());
	mPipeline.reset(new Dx2DPipeline());
	mRootSignature.reset(new Dx2DRootSignature());
	mIndex->CreateIndexBufferView(*DxWrap);

}

Dx2DGraph::~Dx2DGraph() {

}