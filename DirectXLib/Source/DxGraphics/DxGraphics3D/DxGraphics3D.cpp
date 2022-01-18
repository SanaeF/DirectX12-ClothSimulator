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
	std::vector<DxGraphics3D::ModelHandleData> DxGraphics3D::m_Model_data;
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
		int handleID = m_Model_data.size();
		if (m_Model_data.size() <= handleID)m_Model_data.resize(handleID + 1);
		model::PmxModel obj;
		obj.loadFile(file_path); //obj.load("./model/skirt2.fbx");testcloth
		m_Model_data[handleID].model = lib::ModelData::Object;
		m_Model_data[handleID].model.createViewBuffer(m_Dx12->device());
		m_Model_data[handleID].model.createMaterialBuffer(m_Dx12->device());
		m_Texture->loadWIC(lib::ModelData::Object.texturePaths[0].c_str());
		m_Texture->createResource(m_Dx12);
		pipeline.createGraphicsPipeline(m_Dx12, *m_Root_signature, m_Texture->getRootSigDesc());
		m_Model_data[handleID].texture_buffer = m_Texture->getTextureBuff();
		lib::ModelData::Object.initialize();
		return handleID;
	}
	void DxGraphics3D::draw(float x, float y, float z, float size, double Angle, int handle) {
		if (handle == -1)return;
		//if (Handle == -1 || Handle >= mHandleCount)return;
		DrawGraphParam Param;
		Param.x = x / static_cast<float>(m_Dx12->getPixelSize().cx);
		Param.y = y / static_cast<float>(m_Dx12->getPixelSize().cy);
		Param.z = z / static_cast<float>(m_Dx12->getPixelSize().cx);
		Param.size = size;
		Param.angle = Angle;
		createMatrix(handle, m_Dx12->getCount3D(handle));
		drawMatrix(Param, m_Dx12->getCount3D(handle), handle);
		drawCommand(m_Dx12->getCount3D(handle), handle);

		m_Dx12->setCount3D(handle, m_Dx12->getCount3D(handle) + 1);
	}

	void DxGraphics3D::createMatrix(int handle, int num) {
		if (handle == -1)return;
		int ArrySize = num + 1;
		if (m_Model_data[handle].tex_desc_heap_arry.size() <= num &&
			m_Model_data[handle].matrix_arry.size() <= num) {
			m_Model_data[handle].tex_desc_heap_arry.resize(ArrySize);
			m_Model_data[handle].matrix_arry.resize(ArrySize);
		}
		else return;

		m_Texture->descriptorHeap(m_Dx12, m_Texture->getDescriptorHeap());
		m_Texture->shaderResourceView(
			m_Dx12,
			m_Texture->getShaderResourceWierDesc(),
			m_Model_data[handle].texture_buffer,
			m_Texture->getBasicDescHeap()
		);
		m_Matrix->createBuffer3D(*m_Dx12);
		m_Texture->constBuffViwe(
			m_Dx12,
			m_Matrix->getConstBuffer(),
			m_Texture->getDescHandle()
		);
		m_Model_data[handle].tex_desc_heap_arry[num] = m_Texture->getBasicDescHeap();
		m_Model_data[handle].matrix_arry[num] = m_Matrix->getMatData();
	}

	void DxGraphics3D::drawMatrix(DrawGraphParam paramater, int instancedCount, int handle) {
		if (handle == -1)return;
		//mMatrix->update();
		/*if (Paramater.x == 0 &&
			Paramater.y == 0 &&
			Paramater.size == 1 &&
			Paramater.angle == 0
			)return;*/
		m_Matrix->changeMatrix3D(
			m_Model_data[handle].matrix_arry[instancedCount],
			paramater.x,
			paramater.y,
			paramater.z,
			paramater.size,
			paramater.angle
		);
	}
	void DxGraphics3D::drawCommand(int instancedCount, int handle) {
		if (handle == -1)return;
		ID3D12DescriptorHeap* texDH = m_Model_data[handle].tex_desc_heap_arry[instancedCount];
		auto getMatVBView = m_Model_data[handle].model.vb_view;
		auto mat_heap = m_Model_data[handle].model.mat_heap;
		auto getIndexCount = m_Model_data[handle].model.index.size();
		m_Dx12->cmdList()->IASetVertexBuffers(0, 1, &getMatVBView);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &texDH);
		m_Dx12->cmdList()->SetGraphicsRootDescriptorTable(0, texDH->GetGPUDescriptorHandleForHeapStart());
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &mat_heap);
		m_Dx12->cmdList()->DrawIndexedInstanced(getIndexCount, 1, 0, 0, 0);
	}

	void DxGraphics3D::beginDraw(libGraph::DxGraphicsPipeline& pipeline, int handle) {
		if (handle == -1)return;
		auto pipeline_state = pipeline.getPipelineState();
		if (pipeline_state == nullptr)return;
		m_Dx12->cmdList()->SetPipelineState(pipeline_state);

		auto rootsignature = m_Root_signature->getRootSignature();
		if (rootsignature == nullptr)return;
		auto getMatIBView = m_Model_data[handle].model.ib_view;
		m_Dx12->cmdList()->IASetIndexBuffer(&getMatIBView);
		m_Dx12->cmdList()->SetGraphicsRootSignature(rootsignature);
 		m_Dx12->cmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void DxGraphics3D::setupClothPhis(int step, ClothForce world_f, int handle) {
		if (handle == -1)return;
		phy::ClothSimulator::initialize(handle, step, m_Model_data[handle].model, world_f);
	}
	void DxGraphics3D::worldForce(ClothForce world_f, int handle) {
		if (handle == -1)return;
		phy::ClothSimulator::worldForce(handle, world_f);
	}
	void DxGraphics3D::clothUseGPU(int handle, bool flag) {
		if (handle == -1)return;
		phy::ClothSimulator::useGPU(handle, flag);
	}
	void DxGraphics3D::updateClothPhis(int handle) {
		if (handle == -1)return;
		phy::ClothSimulator::execution(
			handle, 
			m_Model_data[handle].model, 
			m_Dx12
		);
	}
	void DxGraphics3D::resetClothPhis(int handle) {
		if (handle == -1)return;
		phy::ClothSimulator::resetModel(handle, m_Model_data[handle].model);
	}
}