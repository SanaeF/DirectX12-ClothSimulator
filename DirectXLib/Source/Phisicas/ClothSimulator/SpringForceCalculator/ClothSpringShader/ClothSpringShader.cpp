#include "ClothSpringShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace phy {
	ClothSpringShader::ClothSpringShader(std::vector<SpringData> spring):
		m_Spring(spring) 
	{
		m_Output_spring = spring;
	}
	void ClothSpringShader::dataInput(lib::ModelData& model, std::vector<SpringData> spring, ComPtr<ID3D12Device> device) {
		m_Spring = spring;
		m_Input.resize(model.vertex.size());
		m_Output_spring = spring;
		for (int ite = 0; ite < m_Input.size(); ite++) {
			m_Input[ite].vert_pos = model.vertex[ite].position;//頂点座標
			m_Input[ite].color = model.vertex[ite].color;//頂点カラー
			//質点ばねモデルのIDグループ
			m_Input[ite].pre_id0 = model.index_group[ite][0];
			m_Input[ite].pre_id1 = model.index_group[ite][1];
			m_Input[ite].pre_id2 = model.index_group[ite][2];
			m_Input[ite].pre_id3 = model.index_group[ite][3];
			m_Input[ite].pre_id4 = model.index_group[ite][4];
			m_Input[ite].pre_id5 = model.index_group[ite][5];
			m_Input[ite].pre_id6 = model.index_group[ite][6];
			m_Input[ite].pre_id7 = model.index_group[ite][7];
			m_pInput[ite] = m_Input[ite];
		}
		createBuffer(device);
	}
	void ClothSpringShader::createCompute(ComPtr<ID3D12Device> device) {
		createRootSig(device);
		createPipeline(device);
		createHeap(device);
		createResource(device);
		createUVA(device);
		resourceMap();
		m_Is_looped = false;
	}
	void ClothSpringShader::execution(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		if (m_Is_looped)m_Input.assign(m_pInput, m_pInput + m_Input.size());

		dx_12->cmdList()->SetComputeRootSignature(m_RootSig);
		dx_12->cmdList()->SetPipelineState(m_Pipeline);
		dx_12->cmdList()->SetDescriptorHeaps(1, &m_Heap);
		auto handle = m_Heap->GetGPUDescriptorHandleForHeapStart();
		dx_12->cmdList()->SetComputeRootDescriptorTable(0, handle);

		//コンピュートシェーダーの実行(今回は256個のスレッドグループを指定)
		dx_12->cmdList()->Dispatch(256, 1, 1);
		m_Is_looped = true;
	}
	bool ClothSpringShader::createBuffer(ComPtr<ID3D12Device> device) {
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(m_Input) + 0xff) & ~0xff);//Helper構造体
		device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_Input_buffer)
		);
		auto result = m_Input_buffer->Map(0, nullptr, (void**)&m_pInput);
		m_Input_buffer->Unmap(0, nullptr);

		return true;
	}
	bool ClothSpringShader::createRootSig(ComPtr<ID3D12Device> device) {
		auto hr = D3DCompileFromFile(L"./DirectXLib/Shader/Shader3D/Cloth/ClothSpring.hlsl", nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, "ClothSpring", "cs_5_1", D3DCOMPILE_DEBUG |
			D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_Shader, nullptr);
		ID3DBlob* sig = nullptr;
		hr = D3DGetBlobPart(m_Shader->GetBufferPointer(), m_Shader->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		//ルートシグネチャの生成
		hr = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
			IID_PPV_ARGS(&m_RootSig));

		return true;
	}
	bool ClothSpringShader::createPipeline(ComPtr<ID3D12Device> device) {
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = m_Shader->GetBufferPointer();
		desc.CS.BytecodeLength = m_Shader->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = m_RootSig;
		auto hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_Pipeline));

		return true;
	}
	bool ClothSpringShader::createHeap(ComPtr<ID3D12Device> device) {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));

		return true;
	}
	bool ClothSpringShader::createResource(ComPtr<ID3D12Device> device) {
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
		desc.Width = (sizeof(SpringData) * m_Output_spring.size() + 0xff) & ~0xff;
		auto hr = device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&m_Resource));

		return true;
	}
	bool ClothSpringShader::createUVA(ComPtr<ID3D12Device> device) {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = m_Output_spring.size();
		desc.Buffer.StructureByteStride = sizeof(float);
		device->CreateUnorderedAccessView(m_Resource, nullptr, &desc, m_Heap->GetCPUDescriptorHandleForHeapStart());

		return true;
	}
	bool ClothSpringShader::resourceMap() {
		D3D12_RANGE range{ 0, 1 };
		auto hr = m_Resource->Map(0, &range, (void**)&m_pOutput_spring);

		return true;
	}
	void ClothSpringShader::output() {
		m_Output_spring.assign(m_pOutput_spring, m_pOutput_spring + m_Output_spring.size());
	}
	std::vector<SpringData> ClothSpringShader::getSpring() {
		return m_Output_spring;
	}
}