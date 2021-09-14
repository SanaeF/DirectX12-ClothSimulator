#include "ClothSpringShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "CSRootSignature/CSRootSignature.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace phy {
	std::vector<float>test(256, 0);
	std::vector<ClothSpringShader::ShaderInfo> ClothSpringShader::shaderHandler;
	std::vector<ClothSpringShader::BufferData> ClothSpringShader::m_Input;

	ClothSpringShader::ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12):
		m_Model_id(model_id),
		m_Dx12(dx_12),
		m_Device(dx_12->device())
	{
		if (shaderHandler.size() <= m_Model_id)shaderHandler.resize(m_Model_id + 1);
		if (m_Input.size() <= m_Model_id)m_Input.resize(m_Model_id + 1);
	}
	void ClothSpringShader::create(
		lib::ModelData& model,
		std::vector<SpringData>& spring,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		m_Output_spring.resize(model.vertex.size());
		createOutput(model, mass_spring_id);

		m_Input[m_Model_id].vertex = model.vertex;
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
		m_Dx12->cmdList()->Dispatch(test.size(), 1, 1);
		//output();
		m_Is_looped = true;
	}
	void ClothSpringShader::output() {
		test.assign((float*)shaderHandler[m_Model_id].data, (float*)shaderHandler[m_Model_id].data + test.size());
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
		createUVA();
		OutputMap();
		shaderHandler[m_Model_id].root_sig = m_RootSig;
		shaderHandler[m_Model_id].pipeline = m_Pipeline;
		shaderHandler[m_Model_id].desc_heap = m_Heap;
		shaderHandler[m_Model_id].output_spring = m_Output_spring;
		shaderHandler[m_Model_id].p_output_spring = m_pOutput_spring;
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
		result = m_Device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
			IID_PPV_ARGS(&m_RootSig));
		//CSRootSignature root(m_Dx12);
		//m_RootSig = root.getResult();
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

		return true;
	}
	bool ClothSpringShader::createHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto hr = m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));

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
		desc.Width = (sizeof(float) * test.size() + 0xff) & ~0xff;
		auto hr = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&m_Resource));

		return true;
	}
	bool ClothSpringShader::createUVA() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = test.size();
		desc.Buffer.StructureByteStride = sizeof(float);
		m_Device->CreateUnorderedAccessView(m_Resource, nullptr, &desc, m_Heap->GetCPUDescriptorHandleForHeapStart());

		return true;
	}
	bool ClothSpringShader::OutputMap() {
		D3D12_RANGE range{ 0, 1 };
		auto hr = m_Resource->Map(0, &range, &shaderHandler[m_Model_id].data);

		return true;
	}

	void ClothSpringShader::createInput() {
		createInputHeap();
		createInputResource();
		createSRV();
		inputMap();
	}
	bool ClothSpringShader::createInputHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto hr = m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&shaderHandler[m_Model_id].input_heap));

		return true;
	}
	bool ClothSpringShader::createInputResource() {
		int elementNum = (shaderHandler[m_Model_id].param.vertex_num * 3) * 2 + shaderHandler[m_Model_id].param.vertex_num * SPRING_NUM;
		auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto desc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(m_Input[m_Model_id]) * elementNum + 0xff) & ~0xff);//Helper構造体
		auto result = m_Device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Input_buffer)
		);
		return true;
	}
	bool ClothSpringShader::createSRV() {
		int elementNum = (shaderHandler[m_Model_id].param.vertex_num * 3) * 2 + shaderHandler[m_Model_id].param.vertex_num * SPRING_NUM;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = elementNum;
		srvDesc.Buffer.StructureByteStride = sizeof(BufferData);
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		m_Device->CreateShaderResourceView(m_Input_buffer, &srvDesc, shaderHandler[m_Model_id].input_heap->GetCPUDescriptorHandleForHeapStart());
		return true;
	}
	bool ClothSpringShader::inputMap() {
		D3D12_RANGE range{ 0, 1 };
		auto result = m_Input_buffer->Map(0, &range, (void**)&m_pInput);

		return true;
	}
	void ClothSpringShader::createMassSpringforGPU(
		lib::ModelData& model, 
		std::vector<std::vector<int>>&
		mass_spring_id
	) {
		shaderHandler[m_Model_id].param.vertex_num = model.vertex.size();
		for (int ite = 0; ite < mass_spring_id.size(); ite++) {
			for (int ite2 = 0; ite2 < SPRING_NUM; ite2++) {
				m_Input[m_Model_id].mass_spring_id[ite2].resize(mass_spring_id.size());
				if (mass_spring_id[ite].size() <= 0) {
					//周囲の質点構造が設定されていない場合
					m_Input[m_Model_id].mass_spring_id[ite2][ite] = -1;
				}
				else m_Input[m_Model_id].mass_spring_id[ite2][ite] = mass_spring_id[ite][ite2];
			}
		}
		m_Input[m_Model_id].pre_vert = model.pre_vert;
	}
	std::vector<SpringData> ClothSpringShader::getSpring() {
		return m_Output_spring;
	}
}