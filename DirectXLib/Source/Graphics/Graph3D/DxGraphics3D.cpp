#include "DxGraphics3D.h"
#include"../../DirectX12/DirectX12Manager.h"
#include"./FbxModel/FbxModel.h"
#include "../Matrix/DxGraphicsMatrix.h"
#include "../GraphPipline/RootSignature/DxRootSignature.h"
#include "../Texture/UploadTexture/DxUploadTex.h"
#include "../GraphPipline/DxGraphicsPipeline.h"

namespace lib {
	int Graph3D::mHandleCount = 0;
	Graph3D::Graph3D(std::shared_ptr<DirectX12Manager>& dx12) :
		mDx12(dx12)
	{
		mMatrix.reset(new libGraph::Dx2DMatrix());
		mTexture.reset(new libGraph::DxUploadTex2D());
		mRootSignature.reset(new libGraph::Dx2DRootSignature());
		fbxData.reset(new model::FbxModel());
		mTexture->DescriptorHeap_Prop();
		mTexture->RootSignatureDesc_Prop();
		mTexture->ShaderResourceViewDesc_Prop();
	}
	int Graph3D::loadFbx(libGraph::Dx2DPipeline& pipeline) {
		int handleID = 0;
		mModelData.resize(handleID + 1);
		mTexture->LoadWIC(L"./dat/shadow_wing.png");
		mTexture->CreateResource(mDx12);
		pipeline.CreateGraphicsPipeline(mDx12, *mRootSignature, mTexture->getRootSigDesc());
		mModelData[handleID].mTextureBuffer = mTexture->getTextureBuff();

		fbxData->load("./model/skirt.fbx");
		fbxData->createViewBuffer(mDx12->Device());
		fbxData->setClothSimulator(mDx12->Device());
		mModelData[handleID].IndexCount = static_cast<UINT>(fbxData->getIndexNum());
		mModelData[handleID].VertexCount = static_cast<UINT>(fbxData->getVertexNum());
		mModelData[handleID].VB = fbxData->getVertexBuffer();
		mModelData[handleID].IB = fbxData->getIndexBuffer();
		mModelData[handleID].vertex = fbxData->getVertex();
		mModelData[handleID].index = fbxData->getIndex();
		mModelData[handleID].VBView = fbxData->getVertexBufferView();
		mModelData[handleID].IBView = fbxData->getIndexBufferView();
		return -1;
	}
	void Graph3D::Draw(float x, float y, float size, double Angle, int Handle) {
		//if (Handle == -1 || Handle >= mHandleCount)return;
		DrawGraphParam Param;
		Param.x = x / static_cast<float>(mDx12->getPixelSize().cx);
		Param.y = y / static_cast<float>(mDx12->getPixelSize().cy);
		Param.size = size;
		Param.angle = Angle;
		mCreateMatrix(Handle, mDx12->getCount3D(Handle));
		mDrawMatrix(Param, mDx12->getCount3D(Handle), Handle);
		mDrawCommand(mDx12->getCount3D(Handle), Handle);

		mDx12->setCount3D(Handle, mDx12->getCount3D(Handle) + 1);
	}

	void Graph3D::mCreateMatrix(int Handle, int num) {
		int ArrySize = num + 1;
		if (mModelData[Handle].TexDescHeapArry.size() <= num &&
			mModelData[Handle].MatrixArry.size() <= num) {
			mModelData[Handle].TexDescHeapArry.resize(ArrySize);
			mModelData[Handle].MatrixArry.resize(ArrySize);
		}
		else return;

		mTexture->mDescriptorHeap(mDx12, mTexture->getDescriptorHeap());
		mTexture->ShaderResourceView(
			mDx12,
			mTexture->getShaderResourceWierDesc(),
			mModelData[Handle].mTextureBuffer,
			mTexture->getBasicDescHeap()
		);
		mMatrix->createBuffer3D(*mDx12);
		mTexture->ConstBuffViwe(
			mDx12,
			mMatrix->getConstBuffer(),
			mTexture->getDescHandle()
		);
		mModelData[Handle].TexDescHeapArry[num] = mTexture->getBasicDescHeap();
		mModelData[Handle].MatrixArry[num] = mMatrix->getMatData();
	}

	void Graph3D::mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {
		//mMatrix->update();
		if (Paramater.x == 0 &&
			Paramater.y == 0 &&
			Paramater.size == 1 &&
			Paramater.angle == 0
			)return;
		mMatrix->ChangeMatrix3D(
			mModelData[Handle].MatrixArry[InstancedCount],
			Paramater.x,
			Paramater.y,
			Paramater.size,
			Paramater.angle
		);
	}
	void Graph3D::mDrawCommand(int InstancedCount, int Handle) {
		ID3D12DescriptorHeap* texDH = mModelData[Handle].TexDescHeapArry[InstancedCount];
		auto getMatVBView = mModelData[Handle].VBView;
		auto getIndexCount = mModelData[Handle].IndexCount;
		mDx12->CmdList()->IASetVertexBuffers(0, 1, &getMatVBView);
		mDx12->CmdList()->SetDescriptorHeaps(1, &texDH);
		mDx12->CmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
		mDx12->CmdList()->DrawIndexedInstanced(getIndexCount, 1, 0, 0, 0);
	}

	void Graph3D::beginDraw(libGraph::Dx2DPipeline& pipeline, int Handle) {
		auto pipeline_state = pipeline.getPipelineState();
		if (pipeline_state == nullptr)return;
		mDx12->CmdList()->SetPipelineState(pipeline_state);

		auto rootsignature = mRootSignature->getRootSignature();
		if (rootsignature == nullptr)return;
		auto getMatIBView = mModelData[Handle].IBView;
		mDx12->CmdList()->IASetIndexBuffer(&getMatIBView);
		mDx12->CmdList()->SetGraphicsRootSignature(rootsignature);
		mDx12->CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void Graph3D::ClothSimProc(int Handle) {
		fbxData->calculatePhysics(mDx12, mModelData[Handle].vertex, mModelData[Handle].index);
		mModelData[Handle].VBView = fbxData->getVertexBufferView();
	}
}