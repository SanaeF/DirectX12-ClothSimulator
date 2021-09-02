#include "DxGraphics3D.h"
#include "ModelLoader/FbxModel/FbxModel.h"
#include "ModelLoader/PmxModel/PmxModel.h"
#include "../../DirectX12Manager/DirectX12Manager.h"
#include "../DxGraphicsMatrix/DxGraphicsMatrix.h"
#include "../DxRootSignature/DxRootSignature.h"
#include "../UploadTexture/UploadTexture.h"
#include "../DxGraphicsPipeline/DxGraphicsPipeline.h"
#include "../../Phisicas/ClothSimulator/ClothSimulator.h"
namespace lib {
	int DxGraphics3D::m_Handle_count = 0;
	DxGraphics3D::DxGraphics3D(std::shared_ptr<DirectX12Manager>& dx_12) :
		m_Dx12(dx_12)
	{
		m_Matrix.reset(new libGraph::Dx2DMatrix());
		m_Texture.reset(new libGraph::UploadTexture());
		m_Root_signature.reset(new libGraph::Dx2DRootSignature());
		m_Texture->descriptorHeapProp();
		m_Texture->rootSignatureDescProp();
		m_Texture->shaderResourceViewDescProp();
	}
	//int DxGraphics3D::loadFbx(const char* path,libGraph::DxGraphicsPipeline& pipeline) {
	//	int handleID = 0;
	//	m_Model_data.resize(handleID + 1);
	//	m_Texture->loadWIC(L"./model/skirt/skirt.png");
	//	m_Texture->createResource(m_Dx12);
	//	pipeline.createGraphicsPipeline(m_Dx12, *m_Root_signature, m_Texture->getRootSigDesc());
	//	m_Model_data[handleID].texture_buffer = m_Texture->getTextureBuff();
	//	model::FbxModel obj;
	//	obj.load(path); //obj.load("./model/skirt2.fbx");testcloth
	//	auto data = obj.getModelData();
	//	data.createViewBuffer(m_Dx12->device(), data);
	//	//obj.setClothSimulator(mDx12->Device());
	//	data = obj.getModelData();
	//	m_Model_data[handleID].vertex = data.vertex;
	//	m_Model_data[handleID].pre_vertex = data.vertex;
	//	m_Model_data[handleID].vb = data.vertex_buffer;
	//	m_Model_data[handleID].vb_view = data.vb_view;
	//	m_Model_data[handleID].index = data.index;
	//	m_Model_data[handleID].ib = data.index_buffer;
	//	m_Model_data[handleID].ib_view = data.ib_view;
	//	m_Model_data[handleID].index_group = data.index_group;
	//	return -1;
	//}
	int DxGraphics3D::loadPmx(const std::wstring& file_path, libGraph::DxGraphicsPipeline& pipeline) {
		int handleID = 0;
		m_Model_data.resize(handleID + 1);
		m_Texture->loadWIC(L"./model/skirt/skirt.png");
		m_Texture->createResource(m_Dx12);
		pipeline.createGraphicsPipeline(m_Dx12, *m_Root_signature, m_Texture->getRootSigDesc());
		m_Model_data[handleID].texture_buffer = m_Texture->getTextureBuff();
		model::PmxModel obj;
		obj.loadFile(file_path); //obj.load("./model/skirt2.fbx");testcloth
		auto data = lib::ModelData::Object;
		data.createViewBuffer(m_Dx12->device());
		//obj.setClothSimulator(mDx12->Device());
		m_Model_data[handleID].vertex = data.vertex;
		m_Model_data[handleID].pre_vertex = data.vertex;
		m_Model_data[handleID].vb = data.vertex_buffer;
		m_Model_data[handleID].vb_view = data.vb_view;
		m_Model_data[handleID].index = data.index;
		m_Model_data[handleID].ib = data.index_buffer;
		m_Model_data[handleID].ib_view = data.ib_view;
		m_Model_data[handleID].index_group = data.index_group;
		return -1;
	}
	void DxGraphics3D::draw(float x, float y, float z, float size, double Angle, int Handle) {
		//if (Handle == -1 || Handle >= mHandleCount)return;
		DrawGraphParam Param;
		Param.x = x / static_cast<float>(m_Dx12->getPixelSize().cx);
		Param.y = y / static_cast<float>(m_Dx12->getPixelSize().cy);
		Param.z = z / static_cast<float>(m_Dx12->getPixelSize().cx);
		Param.size = size;
		Param.angle = Angle;
		createMatrix(Handle, m_Dx12->getCount3D(Handle));
		drawMatrix(Param, m_Dx12->getCount3D(Handle), Handle);
		drawCommand(m_Dx12->getCount3D(Handle), Handle);

		m_Dx12->setCount3D(Handle, m_Dx12->getCount3D(Handle) + 1);
	}

	void DxGraphics3D::createMatrix(int Handle, int num) {
		int ArrySize = num + 1;
		if (m_Model_data[Handle].tex_desc_heap_arry.size() <= num &&
			m_Model_data[Handle].matrix_arry.size() <= num) {
			m_Model_data[Handle].tex_desc_heap_arry.resize(ArrySize);
			m_Model_data[Handle].matrix_arry.resize(ArrySize);
		}
		else return;

		m_Texture->descriptorHeap(m_Dx12, m_Texture->getDescriptorHeap());
		m_Texture->shaderResourceView(
			m_Dx12,
			m_Texture->getShaderResourceWierDesc(),
			m_Model_data[Handle].texture_buffer,
			m_Texture->getBasicDescHeap()
		);
		m_Matrix->createBuffer3D(*m_Dx12);
		m_Texture->constBuffViwe(
			m_Dx12,
			m_Matrix->getConstBuffer(),
			m_Texture->getDescHandle()
		);
		m_Model_data[Handle].tex_desc_heap_arry[num] = m_Texture->getBasicDescHeap();
		m_Model_data[Handle].matrix_arry[num] = m_Matrix->getMatData();
	}

	void DxGraphics3D::drawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle) {
		//mMatrix->update();
		if (Paramater.x == 0 &&
			Paramater.y == 0 &&
			Paramater.size == 1 &&
			Paramater.angle == 0
			)return;
		m_Matrix->changeMatrix3D(
			m_Model_data[Handle].matrix_arry[InstancedCount],
			Paramater.x,
			Paramater.y,
			Paramater.z,
			Paramater.size,
			Paramater.angle
		);
	}
	void DxGraphics3D::drawCommand(int InstancedCount, int Handle) {
		ID3D12DescriptorHeap* texDH = m_Model_data[Handle].tex_desc_heap_arry[InstancedCount];
		auto getMatVBView = m_Model_data[Handle].vb_view;
		auto getIndexCount = m_Model_data[Handle].index.size();
		m_Dx12->cmdList()->IASetVertexBuffers(0, 1, &getMatVBView);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &texDH);
		m_Dx12->cmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
		m_Dx12->cmdList()->DrawIndexedInstanced(getIndexCount, 1, 0, 0, 0);
	}

	void DxGraphics3D::beginDraw(libGraph::DxGraphicsPipeline& pipeline, int Handle) {
		auto pipeline_state = pipeline.getPipelineState();
		if (pipeline_state == nullptr)return;
		m_Dx12->cmdList()->SetPipelineState(pipeline_state);

		auto rootsignature = m_Root_signature->getRootSignature();
		if (rootsignature == nullptr)return;
		auto getMatIBView = m_Model_data[Handle].ib_view;
		m_Dx12->cmdList()->IASetIndexBuffer(&getMatIBView);
		m_Dx12->cmdList()->SetGraphicsRootSignature(rootsignature);
		m_Dx12->cmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void DxGraphics3D::setupClothPhis(int Handle) {
		phy::ClothSimulator sim(m_Model_data[Handle].vertex, m_Model_data[Handle].index, m_Model_data[Handle].index_group);
		m_Model_data[Handle].pre_index = sim.getPreIndexID();
	}
	void DxGraphics3D::updateClothPhis(int Handle) {
		phy::ClothSimulator::update(
			m_Model_data[Handle].vertex,
			m_Model_data[Handle].index,
			m_Model_data[Handle].pre_vertex,
			m_Model_data[Handle].pre_index,
			m_Model_data[Handle].spring_data
		);
		ModelData data;
		data.ib_view = m_Model_data[Handle].ib_view;
		data.index = m_Model_data[Handle].index;
		//data.index_buffer = mModelData[Handle].IB;
		data.vb_view = m_Model_data[Handle].vb_view;
		data.vertex = m_Model_data[Handle].vertex;
		//data.vertex_buffer = mModelData[Handle].VB;
		data.createViewBuffer(m_Dx12->device());

		m_Model_data[Handle].vertex = data.vertex;
		m_Model_data[Handle].vb = data.vertex_buffer;
		m_Model_data[Handle].vb_view = data.vb_view;
		m_Model_data[Handle].index = data.index;
		m_Model_data[Handle].ib = data.index_buffer;
		m_Model_data[Handle].ib_view = data.ib_view;
	}
	void DxGraphics3D::resetClothPhis(int Handle) {
		phy::ClothSimulator::resetPower(m_Model_data[Handle].spring_data);
		ModelData data;
		data.ib_view = m_Model_data[Handle].ib_view;
		data.index = m_Model_data[Handle].index;
		//data.index_buffer = mModelData[Handle].IB;
		data.vb_view = m_Model_data[Handle].vb_view;
		data.vertex = m_Model_data[Handle].pre_vertex;
		//data.vertex_buffer = mModelData[Handle].VB;
		data.createViewBuffer(m_Dx12->device());

		m_Model_data[Handle].vertex = data.vertex;
		m_Model_data[Handle].vb = data.vertex_buffer;
		m_Model_data[Handle].vb_view = data.vb_view;
		m_Model_data[Handle].index = data.index;
		m_Model_data[Handle].ib = data.index_buffer;
		m_Model_data[Handle].ib_view = data.ib_view;
	}
}