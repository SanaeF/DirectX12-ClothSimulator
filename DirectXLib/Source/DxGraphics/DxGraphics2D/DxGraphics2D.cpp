#include "DxGraphics2D.h"
#include "DxGraph2DVertex/DxGraph2DIndex/DxGraph2DIndex.h"
#include "../../DirectX12Manager/DirectX12Manager.h"
#include "../DxGraphicsMatrix/DxGraphicsMatrix.h"
#include "../DxGraphicsPipeline/DxGraphicsPipeline.h"
#include "../DxRootSignature/DxRootSignature.h"
#include "../UploadTexture/UploadTexture.h"
#include<tchar.h>

namespace lib {
	int DxGraphics2D::m_Handle_count = 0;
	//リソース管理の扱い ポール復帰
	int DxGraphics2D::load(const wchar_t* path, libGraph::DxGraphicsPipeline& pipeline) {
		int HandleID = m_Handle_count;
		m_Handle_count++;
		m_Graph_data.resize(m_Handle_count);
		m_Texture->loadWIC(path);
		m_Texture->createResource(m_Dx12);
		pipeline.createGraphicsPipeline(m_Dx12, *m_Root_signature, m_Texture->getRootSigDesc());
		m_Index->setPolygonSize(m_Dx12->getPixelSize(), m_Texture->getMetaData());
		m_Index->createIndexBufferView(*m_Dx12);
		//mViewPort->CreateViewPort(mDxWrap->getPixelSize());
		m_Graph_data[HandleID].tex_buffer = m_Texture->getTextureBuff();
		m_Graph_data[HandleID].vb_view = m_Index->getVertexBufferView();
		//mGraphData[HandleID].mViewPort = mViewPort->getViewPort();
		return HandleID;
	}

	void DxGraphics2D::createMatrix(int Handle, int num) {
		int ArrySize = num + 1;
		if (m_Graph_data[Handle].tex_desc_heap_arry.size() <= num &&
			m_Graph_data[Handle].matrix_arry.size() <= num) {
			m_Graph_data[Handle].tex_desc_heap_arry.resize(ArrySize);
			m_Graph_data[Handle].matrix_arry.resize(ArrySize);
		}
		else return;

		m_Texture->descriptorHeap(m_Dx12, m_Texture->getDescriptorHeap());
		m_Texture->shaderResourceView(
			m_Dx12,
			m_Texture->getShaderResourceWierDesc(),
			m_Graph_data[Handle].tex_buffer,
			m_Texture->getBasicDescHeap()
		);
		m_Matrix->createBuffer(*m_Dx12);//
		m_Texture->constBuffViwe(
			m_Dx12,
			m_Matrix->getConstBuffer(),
			m_Texture->getDescHandle()
		);
		m_Graph_data[Handle].tex_desc_heap_arry[num] = m_Texture->getBasicDescHeap();
		m_Graph_data[Handle].matrix_arry[num] = m_Matrix->getMatData();
	}

	void DxGraphics2D::drawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {
		if (Paramater.x == 0 &&
			Paramater.y == 0 &&
			Paramater.size == 1 &&
			Paramater.angle == 0
			)return;
		m_Matrix->changeMatrix(
			m_Graph_data[Handle].matrix_arry[InstancedCount],
			Paramater.x,
			Paramater.y,
			Paramater.size,
			Paramater.angle
		);
	}

	void DxGraphics2D::drawCommand(int InstancedCount, int Handle) {
		ID3D12DescriptorHeap* texDH = m_Graph_data[Handle].tex_desc_heap_arry[InstancedCount];
		//mDxWrap->CmdList()->RSSetViewports(1, &mGraphData[Handle].mViewPort);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &texDH);
		m_Dx12->cmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
		auto VertexBuff = m_Graph_data[Handle].vb_view;
		m_Dx12->cmdList()->IASetVertexBuffers(0, 1, &VertexBuff);
		m_Dx12->cmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
	void DxGraphics2D::draw(float x, float y, float size, double Angle, int Handle) {
		if (Handle == -1 || Handle >= m_Handle_count)return;
		DrawGraphParam Param;
		Param.x = x / static_cast<float>(m_Dx12->getPixelSize().cx);
		Param.y = y / static_cast<float>(m_Dx12->getPixelSize().cy);
		Param.size = size;
		Param.angle = Angle;

		createMatrix(Handle, m_Dx12->getCount(Handle));
		drawMatrix(Param, m_Dx12->getCount(Handle), Handle);
		drawCommand(m_Dx12->getCount(Handle), Handle);

		m_Dx12->setCount(Handle, m_Dx12->getCount(Handle) + 1);
	}

	void DxGraphics2D::beingDraw(libGraph::DxGraphicsPipeline& pipeline) {
		auto pipeline_state = pipeline.getPipelineState();
		if (pipeline_state == nullptr)return;
		m_Dx12->cmdList()->SetPipelineState(pipeline_state);

		auto rootsignature = m_Root_signature->getRootSignature();
		if (rootsignature == nullptr)return;
		m_Dx12->cmdList()->SetGraphicsRootSignature(rootsignature);
		auto IndexBuff = m_Index->getIndexView();
		m_Dx12->cmdList()->IASetIndexBuffer(&IndexBuff);
		m_Dx12->cmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//TRIANGLELIST
	}

	void DxGraphics2D::setDrawArea(int top, int left, int right, int bottom) {
		//mViewPort->Scissor(top, left, right, bottom);
		//auto getScissor = mViewPort->getScissorrect();
		//mDxWrap->CmdList()->RSSetScissorRects(1, &getScissor);
	}

	void DxGraphics2D::graphicPipeline_Prop() {
		/*mPipeline->LoadShader();
		mPipeline->ShaderState();
		mPipeline->SampleMaskState();
		mPipeline->RasterizerState();
		mPipeline->BlendState();
		mPipeline->Layout();
		mPipeline->RenderTargets();
		mPipeline->AntiAiliasing();*/
	}

	DxGraphics2D::DxGraphics2D(std::shared_ptr<DirectX12Manager>& dx_12) :
		m_Dx12(dx_12),
		m_Copy_type(0)
	{
		m_Texture.reset(new libGraph::UploadTexture());
		m_Matrix.reset(new libGraph::Dx2DMatrix());
		m_Index.reset(new libGraph::DxGraph2DIndex());
		//mPipeline.reset(new libGraph::Dx2DPipeline());
		m_Root_signature.reset(new libGraph::Dx2DRootSignature());
		//mViewPort.reset(new(libGraph::DxViewPort2D));
		m_Index->heapProp();
		m_Texture->descriptorHeapProp();
		m_Texture->rootSignatureDescProp();
		m_Texture->shaderResourceViewDescProp();
		graphicPipeline_Prop();
		setDrawArea(0, 0, 1920, 1440);
	}

	DxGraphics2D::~DxGraphics2D() {

	}
}