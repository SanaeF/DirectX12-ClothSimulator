#include "ClothSpringShader.h"
#include <cassert>
#include <iostream>
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace phy {
	std::vector<float>test(10007, 0);
	std::vector<ClothSpringShader::ShaderInfo> ClothSpringShader::shaderHandler;
	//std::vector<ClothSpringShader::BufferData> ClothSpringShader::m_Input;

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
		if (65535 < model.vertex.size()) {
			assert(0 && "<このモデルは上限を超えています!>DirectX12の使用により、クロスシミュレーターに使用できる頂点数は65535個が限界です。");
		}
		auto& output = shaderHandler[m_Model_id].out_spring;
		shaderHandler[m_Model_id].out_spring.resize(model.vertex.size());
		auto& input = shaderHandler[m_Model_id].in_spring;
		input.resize(model.vertex.size());
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			input[ite].pos = model.vertex[ite].position;
			input[ite].spring = spring[ite];
		}
		input[0].vertex_size = model.vertex.size();
		createOutput(model, mass_spring_id);
		if (!createInput()) {
			shaderHandler[m_Model_id].is_created = false;
			return;
		}
		createOutParam();
		shaderHandler[m_Model_id].is_created = true;
	}
	void ClothSpringShader::execution(int step) {
		m_Dx12->cmdList()->SetComputeRootSignature(shaderHandler[m_Model_id].root_sig);
		m_Dx12->cmdList()->SetPipelineState(shaderHandler[m_Model_id].pipeline);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &shaderHandler[m_Model_id].desc_heap);
		auto handle = shaderHandler[m_Model_id].desc_heap->GetGPUDescriptorHandleForHeapStart();
		m_Dx12->cmdList()->SetComputeRootDescriptorTable(0, handle);
		int size = sqrt(shaderHandler[m_Model_id].in_spring.size());
		int size_out = shaderHandler[m_Model_id].in_spring.size() - (size * size);
		int size_x = size + size_out;
		//コンピュートシェーダーの実行
		m_Dx12->cmdList()->Dispatch(shaderHandler[m_Model_id].thread.x, shaderHandler[m_Model_id].thread.y, 1);
		dataAssign();
	}

	void ClothSpringShader::createOutput(
		lib::ModelData& model,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		if (shaderHandler[m_Model_id].is_created)return;
		createMassSpringforGPU(model, mass_spring_id);
		if (!loadShader())return;
		if (!createPipeline())return;
		if (!createHeap())return;
		if (!createOutputResource())return;
		if (!createUAV())return;
		if (!outputMap())return;
		m_Is_looped = false;
	}
	void ClothSpringShader::createOutParam() {
		if (shaderHandler[m_Model_id].is_created)return;
		shaderHandler[m_Model_id].out_param.resize(1);
		shaderHandler[m_Model_id].out_param[0].max_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		shaderHandler[m_Model_id].out_param[0].min_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		int size = sqrt(shaderHandler[m_Model_id].in_spring.size());
		int size_out = shaderHandler[m_Model_id].in_spring.size() - (size * size);
		auto& thread = shaderHandler[m_Model_id].thread;
		thread.x = size + size_out;
		thread.y = size;
		if (!createParamOutResource())return;
		if (!createParamOutUAV())return;
		if (!outParamMap())return;
	}
	bool ClothSpringShader::createInput() {
		if (!shaderHandler[m_Model_id].is_created) {
			if (!createInputResource())return false;
			if (!createInputUAV())return false;
		}
		if (!inputMap())return false;
		return true;
	}
	void ClothSpringShader::createMassSpringforGPU(
		lib::ModelData& model,
		std::vector<std::vector<int>>&
		mass_spring_id
	) {
		auto& input = shaderHandler[m_Model_id].in_spring;
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			input[ite].pre_pos = model.pre_vert[ite].position;
			input[ite].color = model.pre_vert[ite].color;
			input[ite].id0 = mass_spring_id[ite][0];
			input[ite].id1 = mass_spring_id[ite][1];
			input[ite].id2 = mass_spring_id[ite][2];
			input[ite].id3 = mass_spring_id[ite][3];
			input[ite].id4 = mass_spring_id[ite][4];
			input[ite].id5 = mass_spring_id[ite][5];
			input[ite].id6 = mass_spring_id[ite][6];
			input[ite].id7 = mass_spring_id[ite][7];
			input[ite].id8 = mass_spring_id[ite][8];
			input[ite].id9 = mass_spring_id[ite][9];
			input[ite].id10 = mass_spring_id[ite][10];
			input[ite].id11 = mass_spring_id[ite][11];
		}
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
		result = m_Device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&shaderHandler[m_Model_id].root_sig));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothSpringShader::createPipeline() {
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = m_Shader->GetBufferPointer();
		desc.CS.BytecodeLength = m_Shader->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = shaderHandler[m_Model_id].root_sig;
		auto hr = m_Device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&shaderHandler[m_Model_id].pipeline));

		return true;
	}
	bool ClothSpringShader::createHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 3;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto result = m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&shaderHandler[m_Model_id].desc_heap));
		if (!SUCCEEDED(result))return false;
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
		desc.Width = (sizeof(ClothData) * shaderHandler[m_Model_id].out_spring.size() + 0xff) & ~0xff;
		auto result = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&shaderHandler[m_Model_id].out_spring_res));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothSpringShader::createUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = shaderHandler[m_Model_id].out_spring.size();
		desc.Buffer.StructureByteStride = sizeof(ClothData);

		auto basicHeapHandle = shaderHandler[m_Model_id].desc_heap->GetCPUDescriptorHandleForHeapStart();
		m_Device->CreateUnorderedAccessView(shaderHandler[m_Model_id].out_spring_res, nullptr, &desc, basicHeapHandle);
		basicHeapHandle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		shaderHandler[m_Model_id].desc_handle = basicHeapHandle;
		return true;
	}
	bool ClothSpringShader::outputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pOutputDataBegin;
		auto result = shaderHandler[m_Model_id].out_spring_res->Map(0, &range, &shaderHandler[m_Model_id].spring_data);
		return true;
	}

	bool ClothSpringShader::createInputResource() {
		const UINT buffer_size = static_cast<UINT>(shaderHandler[m_Model_id].in_spring.size()) * sizeof(ClothData);
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
			IID_PPV_ARGS(&shaderHandler[m_Model_id].in_spring_res)
		);
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothSpringShader::createInputUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = shaderHandler[m_Model_id].in_spring.size();
		desc.Buffer.StructureByteStride = sizeof(ClothData);

		m_Device->CreateUnorderedAccessView(shaderHandler[m_Model_id].in_spring_res, nullptr, &desc, shaderHandler[m_Model_id].desc_handle);
		shaderHandler[m_Model_id].desc_handle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		return true;
	}
	bool ClothSpringShader::inputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pInputDataBegin;
		auto& info = shaderHandler[m_Model_id];
		auto result = info.in_spring_res->Map(0, &range, reinterpret_cast<void**>(&pInputDataBegin));
		memcpy(
			pInputDataBegin,
			info.in_spring.data(),
			static_cast<UINT>(info.in_spring.size()) * sizeof(ClothData)
		);
		info.in_spring_res->Unmap(0, nullptr);
		if (!SUCCEEDED(result))return false;
		return true;
	}

	bool ClothSpringShader::createParamOutResource() {
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
		desc.Width = (sizeof(ModelParamater)* shaderHandler[m_Model_id].out_param.size() + 0xff) & ~0xff;
		auto result = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&shaderHandler[m_Model_id].out_param_res));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothSpringShader::createParamOutUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = shaderHandler[m_Model_id].out_param.size();
		desc.Buffer.StructureByteStride = sizeof(ModelParamater);

		m_Device->CreateUnorderedAccessView(shaderHandler[m_Model_id].out_param_res, nullptr, &desc, shaderHandler[m_Model_id].desc_handle);
		shaderHandler[m_Model_id].desc_handle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		return true;
	}
	bool ClothSpringShader::outParamMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pOutputDataBegin;
		auto result = shaderHandler[m_Model_id].out_param_res->Map(0, &range, &shaderHandler[m_Model_id].param_data);
		return true;
	}
	void ClothSpringShader::dataAssign() {
		auto& hd = shaderHandler[m_Model_id];
		hd.out_spring.assign(
			(ClothData*)hd.spring_data,
			(ClothData*)hd.spring_data + hd.out_spring.size());
		hd.out_param.assign(
			(ModelParamater*)hd.param_data,
			(ModelParamater*)hd.param_data + hd.out_param.size());
	}
	void ClothSpringShader::dataChange(
		int model_id, lib::ModelData& model, std::vector<SpringData>& spring
	){
		auto& output = shaderHandler[m_Model_id].out_spring;
		if (!output[0].simulate)return;
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			model.vertex[ite].position = output[ite].pos;
			spring[ite].velocity = output[ite].spring.velocity;
		}
	}
	ClothSpringShader::ModelParamater 
	ClothSpringShader::getMaxMinPos(int id) {
		return shaderHandler[id].out_param[0];
	}
	ClothSpringShader::~ClothSpringShader() {
	}
}