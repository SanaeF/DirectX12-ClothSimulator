#include "ClothSpringShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
#include "CSRootSignature/CSRootSignature.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace phy {
	std::vector<float>test(10007, 0);
	std::vector<ClothSpringShader::ShaderInfo> ClothSpringShader::shaderHandler;
	std::vector<ClothSpringShader::BufferData> ClothSpringShader::m_Input;

	ClothSpringShader::ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12):
		m_Model_id(model_id),
		m_Dx12(dx_12),
		m_Device(dx_12->device())
	{
		if (shaderHandler.size() <= m_Model_id)shaderHandler.resize(m_Model_id + 1);
	}
	void ClothSpringShader::create(
		lib::ModelData& model,
		std::vector<SpringData>& spring,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		m_Input.resize(model.vertex.size());
		shaderHandler[m_Model_id].output.resize(model.vertex.size());
		createOutput(model, mass_spring_id);
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			m_Input[ite].pos = model.vertex[ite].position;
			m_Input[ite].spring = spring[ite];
		}
		createInput();
	}
	void ClothSpringShader::execution() {
		if (m_Is_looped)output();
		m_Dx12->cmdList()->SetComputeRootSignature(shaderHandler[m_Model_id].root_sig);
		m_Dx12->cmdList()->SetPipelineState(shaderHandler[m_Model_id].pipeline);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &shaderHandler[m_Model_id].desc_heap);
		auto handle = shaderHandler[m_Model_id].desc_heap->GetGPUDescriptorHandleForHeapStart();
		m_Dx12->cmdList()->SetComputeRootDescriptorTable(0, handle);
		//コンピュートシェーダーの実行(今回は256個のスレッドグループを指定)
		m_Dx12->cmdList()->Dispatch(m_Input.size(), 1, 1);

		//output();
		m_Is_looped = true;
	}
	void ClothSpringShader::output() {
		auto& data = shaderHandler[m_Model_id];
		data.output.assign(
			(BufferData*)data.data,
			(BufferData*)data.data + data.output.size());
	}

	void ClothSpringShader::createOutput(
		lib::ModelData& model,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		if (shaderHandler[m_Model_id].is_created)return;
		createMassSpringforGPU(model, mass_spring_id);
		loadShader();
		createPipeline();
		createHeap();
		createOutputResource();
		createUAV();
		outputMap();
		shaderHandler[m_Model_id].is_created = true;
		m_Is_looped = false;
	}
	bool ClothSpringShader::loadShader() {
		auto result = D3DCompileFromFile(
			L"./DirectXLib/Shader/Shader3D/Cloth/ClothSpring.hlsl", nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, "ClothSpring", "cs_5_1",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &m_Shader, nullptr
		);
		ID3DBlob* sig = nullptr;
		result = D3DGetBlobPart(m_Shader->GetBufferPointer(), m_Shader->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		//ルートシグネチャの生成
		result = m_Device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_RootSig));
		shaderHandler[m_Model_id].root_sig = m_RootSig;
		return true;
	}
	bool ClothSpringShader::createPipeline() {
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = m_Shader->GetBufferPointer();
		desc.CS.BytecodeLength = m_Shader->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = m_RootSig;
		auto hr = m_Device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_Pipeline));
		shaderHandler[m_Model_id].pipeline = m_Pipeline;

		return true;
	}
	bool ClothSpringShader::createHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 2;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto result = m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
		shaderHandler[m_Model_id].desc_heap = m_Heap;

		return true;
	}
	bool ClothSpringShader::createOutputResource() {
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		//サイズは定数バッファと同じように指定
		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = (sizeof(BufferData) * shaderHandler[m_Model_id].output.size() + 0xff) & ~0xff;
		auto result = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&m_Resource));
		shaderHandler[m_Model_id].resource = m_Resource;
		return true;
	}
	bool ClothSpringShader::createUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = shaderHandler[m_Model_id].output.size();
		desc.Buffer.StructureByteStride = sizeof(BufferData);

		auto basicHeapHandle = m_Heap->GetCPUDescriptorHandleForHeapStart();
		m_Device->CreateUnorderedAccessView(m_Resource, nullptr, &desc, basicHeapHandle);
		basicHeapHandle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		shaderHandler[m_Model_id].desc_handle = basicHeapHandle;
		return true;
	}
	bool ClothSpringShader::outputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pOutputDataBegin;
		auto result = m_Resource->Map(0, &range, &shaderHandler[m_Model_id].data);
		return true;
	}
	void ClothSpringShader::createInput() {
		createInputResource();
		createInputUAV();
		inputMap();
	}
	bool ClothSpringShader::createInputResource() {
		const UINT buffer_size = static_cast<UINT>(m_Input.size()) * sizeof(BufferData);
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = buffer_size;
		auto result = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Input_buffer)
		);
		return true;
	}
	bool ClothSpringShader::createInputUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = m_Input.size();
		desc.Buffer.StructureByteStride = sizeof(BufferData);
		m_Device->CreateUnorderedAccessView(m_Input_buffer, nullptr, &desc, shaderHandler[m_Model_id].desc_handle);
		return true;
	}
	bool ClothSpringShader::inputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pInputDataBegin;
		auto result = m_Input_buffer->Map(0, &range, reinterpret_cast<void**>(&pInputDataBegin));
		memcpy(pInputDataBegin, m_Input.data(), static_cast<UINT>(m_Input.size()) * sizeof(BufferData));
		m_Input_buffer->Unmap(0, nullptr);
		return true;
	}

	//bool ClothSpringShader::createInputHeap() {
	//	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//	desc.NodeMask = 0;
	//	desc.NumDescriptors = 1;
	//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	auto result = m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&shaderHandler[m_Model_id].input_heap));

	//	return true;
	//}
	//bool ClothSpringShader::createInputBuffer() {
	//	int elementNum = m_Input.size();
	//	const UINT buffer_size = static_cast<UINT>(elementNum) * sizeof(BufferData);
	//	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//	auto desc = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);
	//	auto result = m_Device->CreateCommittedResource(
	//		&heap,
	//		D3D12_HEAP_FLAG_NONE,
	//		&desc,
	//		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	//		IID_PPV_ARGS(&m_Input_buffer));//ファイルが無いとエラー
	//	D3D12_RANGE range{ 0, 0 };
	//	UINT8* pInputDataBegin;
	//	result = m_Input_buffer->Map(0, &range, reinterpret_cast<void**>(&pInputDataBegin));
	//	memcpy(pInputDataBegin, m_Input.data(), buffer_size);
	//	//m_Input_buffer->Unmap(0, nullptr);
	//	return true;
	//}
	//bool ClothSpringShader::createSRV() {
	//	int elementNum = m_Input.size() * (SPRING_NUM + 2);
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//	ZeroMemory(&srvDesc, sizeof(srvDesc));
	//	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//	srvDesc.Texture2D.MipLevels = 1;
	//	srvDesc.Buffer.FirstElement = 0;
	//	srvDesc.Buffer.NumElements = elementNum;
	//	srvDesc.Buffer.StructureByteStride = sizeof(BufferData);
	//	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	//	m_Device->CreateShaderResourceView(m_Input_buffer, &srvDesc, shaderHandler[m_Model_id].desc_handle);
	//	//auto result = m_Device->GetDeviceRemovedReason();
	//	return true;
	//}
	void ClothSpringShader::createMassSpringforGPU(
		lib::ModelData& model, 
		std::vector<std::vector<int>>&
		mass_spring_id
	) {

		for (int ite = 0; ite < model.vertex.size(); ite++) {
			m_Input[ite].pre_pos = model.pre_vert[ite].position;
			m_Input[ite].color = model.pre_vert[ite].color;
			m_Input[ite].id0 = mass_spring_id[ite][0];
			m_Input[ite].id1 = mass_spring_id[ite][1];
			m_Input[ite].id2 = mass_spring_id[ite][2];
			m_Input[ite].id3 = mass_spring_id[ite][3];
			m_Input[ite].id4 = mass_spring_id[ite][4];
			m_Input[ite].id5 = mass_spring_id[ite][5];
			m_Input[ite].id6 = mass_spring_id[ite][6];
			m_Input[ite].id7 = mass_spring_id[ite][7];
		}
		//shaderHandler[m_Model_id].param.vertex_num = model.vertex.size();
		//for (int ite = 0; ite < mass_spring_id.size(); ite++) {
		//	for (int ite2 = 0; ite2 < SPRING_NUM; ite2++) {
		//		m_Input[m_Model_id].mass_spring_id[ite2].resize(mass_spring_id.size());
		//		if (mass_spring_id[ite].size() <= 0) {
		//			//周囲の質点構造が設定されていない場合
		//			m_Input[m_Model_id].mass_spring_id[ite2][ite] = -1;
		//		}
		//		else m_Input[m_Model_id].mass_spring_id[ite2][ite] = mass_spring_id[ite][ite2];
		//	}
		//}
		//m_Input[m_Model_id].pre_vert = model.pre_vert;
	}
	void ClothSpringShader::dataChange(
		int model_id, lib::ModelData& model, std::vector<SpringData>& spring
	){
		if (model.vertex.size() != shaderHandler[m_Model_id].output.size())return;
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			model.vertex[ite].position = shaderHandler[m_Model_id].output[ite].pos;
			spring[ite] = shaderHandler[m_Model_id].output[ite].spring;
		}
	}
}