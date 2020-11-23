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

int Dx2DGraph::Load(const wchar_t* path) {
	int HandleID = mHandleCount; 
	mHandleCount++;
	mGraphData.resize(mHandleCount);
	mTexture->LoadWIC(path);
	mTexture->CreateResource(mDxWrap);
	mPipeline->CreateGraphicsPipeline(mDxWrap, *mRootSignature, mTexture->getRootSigDesc());
	mIndex->setPolygonSize(mDxWrap->getPixelSize(), mTexture->getMetaData());
	mIndex->CreateIndexBufferView(*mDxWrap);
	mViewPort->CreateViewPort(mDxWrap->getPixelSize(), mTexture->getMetaData());
	mGraphData[HandleID].mTextureBuffer = mTexture->getTextureBuff();
	mGraphData[HandleID].mVertexBufferView = mIndex->getvertexBufferView();
	mGraphData[HandleID].mViewPort = mViewPort->getViewPort();
	return HandleID;
}

void Dx2DGraph::mCreateMatrix(int Handle, int num) {
	int ArrySize = num + 1;
	if (mGraphData[Handle].TexDescHeapArry.size() <= num &&
		mGraphData[Handle].MatrixArry.size() <= num) {
		mGraphData[Handle].TexDescHeapArry.resize(ArrySize);
		mGraphData[Handle].MatrixArry.resize(ArrySize);
	}
	else return;

	mTexture->mDescriptorHeap(mDxWrap, mTexture->getDescriptorHeap());
	mTexture->ShaderResourceView(
		mDxWrap,
		mTexture->getShaderResourceWierDesc(),
		mGraphData[Handle].mTextureBuffer,
		mTexture->getBasicDescHeap()
	);
	mMatrix->createBuffer(*mDxWrap);//
	mTexture->ConstBuffViwe(
		mDxWrap,
		mMatrix->getConstBuffer(),
		mTexture->getDescHandle()
	);
	mGraphData[Handle].TexDescHeapArry[num] = mTexture->getBasicDescHeap();
	mGraphData[Handle].MatrixArry[num] = mMatrix->getMatData();
}

void Dx2DGraph::mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {
	if (Paramater.x == 0 &&
		Paramater.y == 0 && 
		Paramater.size == 1 &&
		Paramater.angle == 0
		)return;
	mMatrix->ChangeMatrix(
		mGraphData[Handle].MatrixArry[InstancedCount],
		Paramater.x, 
		Paramater.y, 
		Paramater.size, 
		Paramater.angle
	);
}

void Dx2DGraph::mDrawCommand(int InstancedCount, int Handle) {
	ID3D12DescriptorHeap* texDH = mGraphData[Handle].TexDescHeapArry[InstancedCount];
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

	mCreateMatrix(Handle, mDxWrap->getCount(Handle));
	mDrawMatrix(Param, mDxWrap->getCount(Handle), Handle);
	mDrawCommand(mDxWrap->getCount(Handle), Handle);

	mDxWrap->setCount(Handle, mDxWrap->getCount(Handle) + 1);
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

void Dx2DGraph::SetDrawArea(int top, int left, int right, int bottom) {
	mViewPort->Scissor(top, left, right, bottom);
	auto getScissor = mViewPort->getScissorrect();
	mDxWrap->CmdList()->RSSetScissorRects(1, &getScissor);
}

void Dx2DGraph::GraphicPipeline_Prop() {
	mPipeline->LoadShader();
	mPipeline->ShaderState();
	mPipeline->SampleMaskState();
	mPipeline->RasterizerState();
	mPipeline->BlendState();
	mPipeline->Layout();
	mPipeline->RenderTargets();
	mPipeline->AntiAiliasing();
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
	mIndex->Heap_Prop();
	mTexture->DescriptorHeap_Prop();
	mTexture->RootSignatureDesc_Prop();
	mTexture->ShaderResourceViewDesc_Prop();
	GraphicPipeline_Prop();
	SetDrawArea(0, 0, 1920, 1440);
}

Dx2DGraph::~Dx2DGraph() {

}