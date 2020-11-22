#include "Dx2DGraph.h"
#include"Dx12Wrapper.h"
#include "DxViewPort2D.h"	
#include "DxUploadTex2D.h"
#include "Dx2DMatrix.h"
#include "DxIndex2D.h"
#include "Dx2DRootSignature.h"
#include "Dx2DPipeline.h"
#include<tchar.h>

int Dx2DGraph::mHandleCount = 0;
//リソース管理の扱い ポール復帰

void inline Dx2DGraph::CopyTex(int num) {
	if (num == eTYPE_UMA) {
		mTexture->CreateResource(mDxWrap);
		mTexture->mDescriptorHeap(mDxWrap, mTexture->getDescriptorHeap());
		mTexture->ShaderResourceView(
			mDxWrap,
			mTexture->getShaderResourceWierDesc(),
			mTexture->getTextureBuff(),
			mTexture->getBasicDescHeap()
		);
	}
	if (num == eTYPE_DMA) {
		mTexture->CreateTexBuffer(mDxWrap);
		mTexture->ResourceUpload();
		mTexture->TextureCopy(mDxWrap);
		mTexture->BarrierDescProp(mDxWrap);
		mTexture->ShaderResourceViewforUpload(mDxWrap);
	}
}

int Dx2DGraph::Load(const wchar_t* path) {
	const int Max = 500;
	int HandleID = mHandleCount; mHandleCount += Max; mGraphData.resize(HandleID + Max + 1);

	mTexture->LoadWIC(path);
	mTexture->CreateResource(mDxWrap);

	for (int i = 0; i < Max; i++) {
		mTexture->mDescriptorHeap(mDxWrap, mTexture->getDescriptorHeap());
		mTexture->ShaderResourceView(
			mDxWrap,
			mTexture->getShaderResourceWierDesc(),
			mTexture->getTextureBuff(),
			mTexture->getBasicDescHeap()
		);
		mMatrix->createBuffer(*mDxWrap);//
		mTexture->ConstBuffViwe(
			mDxWrap,
			mMatrix->getConstBuffer(),
			mTexture->getDescHandle()
		);
		mGraphData[HandleID + i].mTexDescHeap = mTexture->getBasicDescHeap();
		mGraphData[HandleID + i].mMatrix = mMatrix->getMatData();
	}

	mPipeline->CreateGraphicsPipeline(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	mIndex->setPolygonSize(mDxWrap->getPixelSize(), mTexture->getMetaData());
	mIndex->CreateIndexBufferView(*mDxWrap);
	mViewPort->CreateViewPort(mDxWrap->getPixelSize(), mTexture->getMetaData());
	mGraphData[HandleID].mVertexBufferView = mIndex->getvertexBufferView();
	mGraphData[HandleID].mViewPort = mViewPort->getViewPort();


	return HandleID;
}


void inline Dx2DGraph::mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {

	//mMatrix->BasicChangeMatrix(
	//	InstancedCount,
	//	Paramater.x,
	//	Paramater.y,
	//	Paramater.size,
	//	Paramater.angle
	//);
	mMatrix->ChangeMatrix(
		InstancedCount, 
		mGraphData[Handle+ InstancedCount-1].mMatrix,
		Paramater.x, 
		Paramater.y, 
		Paramater.size, 
		Paramater.angle
	);
}

void inline Dx2DGraph::mDrawCommand(int InstancedCount, int Handle) {
	ID3D12DescriptorHeap* texDH = mGraphData[Handle + InstancedCount - 1].mTexDescHeap;
	mDxWrap->CmdList()->RSSetViewports(1, &mGraphData[Handle].mViewPort);
	mDxWrap->CmdList()->SetDescriptorHeaps(1, &texDH);
	mDxWrap->CmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
	auto VertexBuff = mGraphData[Handle].mVertexBufferView;
	mDxWrap->CmdList()->IASetVertexBuffers(0, 1, &VertexBuff);
	mDxWrap->CmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Dx2DGraph::Draw(float x, float y, float size, double Angle, int Handle) {
	if (Handle == -1 || Handle >= mHandleCount)return;
	DrawGraphParam Param;
	Param.x = x / static_cast<float>(mDxWrap->getPixelSize().cx);
	Param.y = y / static_cast<float>(mDxWrap->getPixelSize().cy);
	Param.size = size;
	Param.angle = Angle;

	mDxWrap->setCount(Handle, mDxWrap->getCount(Handle) + 1);

	mDrawMatrix(Param, mDxWrap->getCount(Handle), Handle);
	mDrawCommand(mDxWrap->getCount(Handle), Handle);

}

void Dx2DGraph::BeingDraw() {
	auto pipeline_state = mPipeline->getPipelineState();
	mDxWrap->CmdList()->SetPipelineState(pipeline_state);

	auto rootsignature = mRootSignature->getRootSignature();
	mDxWrap->CmdList()->SetGraphicsRootSignature(rootsignature);

	auto IndexBuff = mIndex->getIndexView();
	mDxWrap->CmdList()->IASetIndexBuffer(&IndexBuff);
	mDxWrap->CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//TRIANGLELIST
}

void Dx2DGraph::setMatrix() {
	//for(int i=0;i< mGraphData.size();i++)mMatrix->setMatrix(mGraphData[i].mMatrix);
}

void Dx2DGraph::SetDrawArea(int top, int left, int right, int bottom) {
	mViewPort->Scissor(top, left, right, bottom);
	auto getScissor = mViewPort->getScissorrect();
	mDxWrap->CmdList()->RSSetScissorRects(1, &getScissor);
}

void inline Dx2DGraph::GraphicPipeline() {
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

Dx2DGraph::Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap) :
	mDxWrap(DxWrap) , 
	CopyType(0)
{
	mTexture.reset(new DxUploadTex2D());
	mMatrix.reset(new Dx2DMatrix());
	mIndex.reset(new DxIndex2D());
	mPipeline.reset(new Dx2DPipeline());
	mRootSignature.reset(new Dx2DRootSignature());
	mViewPort.reset(new(DxViewPort2D));
	//mTexture->getDirectXWrap(mDxWrap);
	mTexture->DescriptorHeap();
	mTexture->RootSignatureDesc();
	mTexture->ShaderResourceViewDesc();
	GraphicPipeline();
	SetDrawArea(0, 0, 1920, 1440);
}

Dx2DGraph::~Dx2DGraph() {

}

//mTexture->ConstBuffViwe(mDxWrap, mMatrix->getConstBuffer(), mTexture->getDescHandle());
//mGraphData[HandleID].mDescriptorHeap = mTexture->getDescriptorHeap();
//mGraphData[HandleID].mTextureData = mTexture->getMetaData();
//mGraphData[HandleID].mViewPort = mViewPort->getViewPort();
//mGraphData[HandleID].mRootSignature = mRootSignature->getRootSignature();
//mGraphData[HandleID].mPipeline = mPipeline->getPipeline();
//mGraphData[HandleID].mTextureBuffer = mTexture->getTextureBuff();
//mGraphData[HandleID].mTexDescHeap = mTexture->getBasicDescHeap();
//mGraphData[HandleID].mDescHandle = mTexture->getDescHandle();