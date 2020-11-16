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

void inline Dx2DGraph::CopyTex(int num, int Handle) {
	if (num == eTYPE_UMA) {
		mTexture->CreateResource(mDxWrap);
		mTexture->mDescriptorHeap(mDxWrap);
		mTexture->ShaderResourceView(mDxWrap, mMatrix->getConstBuffer());
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
	int HandleID = mHandleCount;mHandleCount++;mGraphData.resize(HandleID + 1);

	mMatrix->createBuffer(*mDxWrap);//
	mTexture->LoadWIC(path);
	CopyTex(eTYPE_UMA, HandleID);
	mPipeline->RootSignatureState(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	mPipeline->CreateGraphicsPipeline(mDxWrap);
	mViewPort->CreateViewPort(mDxWrap->getPixelSize(), mTexture->getMetaData());

	DxIndex2D mIndex;
	mIndex.setPolygonSize(mDxWrap->getPixelSize(), mTexture->getMetaData());
	mIndex.CreateIndexBufferView(*mDxWrap);

	mGraphData[HandleID].mMatrix = mMatrix->getMatData();
	mGraphData[HandleID].mViewPort = mViewPort->getViewPort();
	mGraphData[HandleID].mRootSignature = mRootSignature->getRootSignature();
	mGraphData[HandleID].mPipelineState = mPipeline->getPipelineState();
	mGraphData[HandleID].mTexDescHeap = mTexture->getBasicDescHeap();
	mGraphData[HandleID].mVertexBufferView = mIndex.getvertexBufferView();
	mGraphData[HandleID].mVertexIndexView = mIndex.getIndexView();

	return HandleID;
}


void inline Dx2DGraph::mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {
	mMatrix->ChangeMatrix(
		InstancedCount, 
		mGraphData[Handle].mMatrix,
		Paramater.x, 
		Paramater.y, 
		Paramater.size, 
		Paramater.angle
	);
	//mMatrix->Rotation(InstancedCount, *mGraphData[Handle].mMatrix, 0, 0, Paramater.angle);
}

void inline Dx2DGraph::mDrawCommand(int InstancedCount, int Handle) {
	ID3D12DescriptorHeap* texDH = mGraphData[Handle].mTexDescHeap;
	mDxWrap->CmdList()->RSSetViewports(1, &mGraphData[Handle].mViewPort);
	mDxWrap->CmdList()->SetPipelineState(mGraphData[Handle].mPipelineState);
	mDxWrap->CmdList()->SetGraphicsRootSignature(mGraphData[Handle].mRootSignature);
	mDxWrap->CmdList()->SetDescriptorHeaps(1, &texDH);
	mDxWrap->CmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
	mDxWrap->CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//TRIANGLELIST

	mDxWrap->CmdList()->IASetVertexBuffers(0, 1, &mGraphData[Handle].mVertexBufferView);
	mDxWrap->CmdList()->IASetIndexBuffer(&mGraphData[Handle].mVertexIndexView);
	if(InstancedCount>1)mDxWrap->CmdList()->DrawIndexedInstanced(6, 210, 0, 0, 0);
	else mDxWrap->CmdList()->DrawIndexedInstanced(6, InstancedCount, 0, 0, 0);
	

}

void Dx2DGraph::Draw(float x, float y, float size, double Angle, int Handle) {
	if (Handle == -1 || Handle >= mHandleCount)return;
	DrawGraphParam Param;
	Param.x = x;
	Param.y = y;
	Param.size = size;
	Param.angle = Angle;

	mDxWrap->setCount(Handle, mDxWrap->getCount(Handle) + 1);

	mDrawMatrix(Param, mDxWrap->getCount(Handle), Handle);
	mDrawCommand(mDxWrap->getCount(Handle), Handle);

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
	//mIndex.reset(new DxIndex2D());
	mPipeline.reset(new Dx2DPipeline());
	mRootSignature.reset(new Dx2DRootSignature());
	mViewPort.reset(new(DxViewPort2D));

	GraphicPipeline();
	SetDrawArea(0, 0, 1920, 1440);
}

Dx2DGraph::~Dx2DGraph() {

}