#include "Dx2DGraph.h"
#include"Dx12Wrapper.h"
#include "DxViewPort2D.h"	
#include "DxUploadTex2D.h"
#include "Dx2DMatrix.h"
#include "DxIndex2D.h"
#include "Dx2DRootSignature.h"
#include "Dx2DPipeline.h"

int Dx2DGraph::mHandleCount = 0;

int Dx2DGraph::Load(const wchar_t* path) {
	//DxIndex2D mIndex;

	int HandleID = mHandleCount;
	mGraphData.resize(HandleID + 1);
	//CopyType = eTYPE_DMA;

	mMatrix->createBuffer(*mDxWrap);//
	mTexture->LoadWIC(path);
	//mIndex.setPolygonSize(mDxWrap->getPixelSize(), mTexture->getMetaData());
	//mIndex.CreateIndexBufferView(*mDxWrap);
	CopyTex(eTYPE_UMA, HandleID);
	mPipeline->RootSignatureState(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	mPipeline->CreateGraphicsPipeline(mDxWrap);

	mViewPort->CreateViewPort(mDxWrap->getPixelSize(), mTexture->getMetaData());

	SetHandleData(HandleID);
	//mGraphData[HandleID].mVertexBufferView = mIndex.getvertexBufferView();
	//mGraphData[HandleID].mVertexIndexView = mIndex.getIndexView();


	return HandleID;
}

void Dx2DGraph::Draw(double x, double y, double Angle, int Handle) {
	if (Handle == -1 || Handle >= mHandleCount)return;
	DxIndex2D mIndex;

	mIndex.setPolygonSize(mDxWrap->getPixelSize(), mGraphData[Handle].mMetaData);
	mIndex.CreateIndexBufferView(*mDxWrap);

	mMatrix->Rotation(*mGraphData[Handle].mMatrix, 0, 0, Angle);
	//mMatrix->TransMove(*mGraphData[Handle].mMatrix, x, y, 0);

	mDxWrap->CommandList()->RSSetViewports(1, &mGraphData[Handle].mViewPort);
	mDxWrap->CommandList()->RSSetScissorRects(1, &mViewPort->getScissorrect());
	mDxWrap->CommandList()->SetPipelineState(mGraphData[Handle].mPipelineState);
	mDxWrap->CommandList()->SetGraphicsRootSignature(mGraphData[Handle].mRootSignature);

	ID3D12DescriptorHeap* mTexDescHeap = mGraphData[Handle].mTexDescHeap;
	mDxWrap->CommandList()->SetDescriptorHeaps(1, &mTexDescHeap);
	mDxWrap->CommandList()->SetGraphicsRootDescriptorTable(
		0,
		mTexDescHeap->GetGPUDescriptorHandleForHeapStart()
	);
	auto heapHandle = mTexDescHeap->GetGPUDescriptorHandleForHeapStart();
	heapHandle.ptr += mDxWrap->Device()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	);
	mDxWrap->CommandList()->SetGraphicsRootDescriptorTable(1, heapHandle);
	mDxWrap->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDxWrap->CommandList()->IASetVertexBuffers(0, 1, &mIndex.getvertexBufferView());
	mDxWrap->CommandList()->IASetIndexBuffer(&mIndex.getIndexView());
	mDxWrap->CommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Dx2DGraph::SetDrawArea(int top, int left, int right, int bottom) {
	mViewPort->Scissor(top, left, right, bottom);
}


void Dx2DGraph::CopyTex(int num, int Handle) {
	if (num == eTYPE_UMA) {
		mTexture->CreateResource(mDxWrap);
		mTexture->ShaderResourceView(mDxWrap, mMatrix->getConstBuffer());//
	}
	if (num == eTYPE_DMA) {
		mTexture->CreateTexBuffer(mDxWrap);

		mTexture->ResourceUpload();

		mTexture->TextureCopy(mDxWrap);

		mTexture->BarrierDescProp(mDxWrap);

		mTexture->ShaderResourceViewforUpload(mDxWrap);
	}
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
	//mPipeline->RootSignatureState(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	//mPipeline->CreateGraphicsPipeline(mDxWrap);
}

void Dx2DGraph::SetHandleData(int Handle) {
	mGraphData[Handle].mResBuf = mMatrix->getConstBuffer();
	mGraphData[Handle].mMatrix = mMatrix->getMatDefault();
	mGraphData[Handle].mTexDescHeap = mTexture->getBasicDescHeap();
	mGraphData[Handle].mPipelineState = mPipeline->getPipelineState();
	mGraphData[Handle].mRootSignature = mRootSignature->getRootSignature();
	//mGraphData[Handle].mVertexBufferView = mIndex->getvertexBufferView();
	//mGraphData[Handle].mVertexIndexView = mIndex->getIndexView();
	mGraphData[Handle].mViewPort = mViewPort->getViewPort();
	mGraphData[Handle].CopyType = CopyType;
	mGraphData[Handle].mMetaData = mTexture->getMetaData();
	mHandleCount++;
}


Dx2DGraph::Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap) :
	mDxWrap(DxWrap) , 
	CopyType(0)
{
	mTexture.reset(new DxUploadTex2D());
	mMatrix.reset(new Dx2DMatrix());
	//mIndex.reset(new DxIndex2D());
	mPipeline.reset(new Dx2DPipeline());
	mRootSignature.reset(new Dx2DRootSignature());
	mViewPort.reset(new(DxViewPort2D));

	GraphicPipeline();
}

Dx2DGraph::~Dx2DGraph() {

}