#include "ComputeShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
namespace lib {
	std::vector<ComputeShader::ComputeShaderInfo> ComputeShader::m_Shader_handle;
	int ComputeShader::m_Handle_count = 0;
	ComputeShader::ComputeShader(
		const char* file_path, 
		const char* func_name, 
		int slot_max, 
		int& handle,
		std::shared_ptr<lib::DirectX12Manager> dx12
	):
		m_Dx12(dx12)
	{
		m_Handle_id = m_Handle_count;
		m_Handle_count++;
		m_Shader_handle.resize(m_Handle_count);
		m_Shader_handle[m_Handle_id].resource.resize(slot_max);
		m_Shader_handle[m_Handle_id].element.resize(slot_max);
		m_Shader_handle[m_Handle_id].data.resize(slot_max);
		loadShaderFile(file_path, func_name);
		createPipline();
		createHeap(slot_max);
		handle = m_Handle_id;
	}
	ComputeShader::ComputeShader(int handle, std::shared_ptr<lib::DirectX12Manager> dx12):
		m_Handle_id(handle),
		m_Dx12(dx12)
	{

	}
	void ComputeShader::loadShaderFile(const char* file_path, const char* func_name) {
		wchar_t w_file_path[256] = { L"" };
		mbstowcs(w_file_path, file_path, 256);
		auto result = D3DCompileFromFile(
			w_file_path, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, func_name, "cs_5_1",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &m_Shader_handle[m_Handle_id].shader, nullptr
		);
		ID3DBlob* sig = nullptr;
		result = D3DGetBlobPart(
			m_Shader_handle[m_Handle_id].shader->GetBufferPointer(), 
			m_Shader_handle[m_Handle_id].shader->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		//ルートシグネチャの生成
		result = m_Dx12->device()->CreateRootSignature(
			0, 
			sig->GetBufferPointer(), 
			sig->GetBufferSize(), 
			IID_PPV_ARGS(&m_Shader_handle[m_Handle_id].root_sig)
		);
	}
	void ComputeShader::createPipline() {
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = m_Shader_handle[m_Handle_id].shader->GetBufferPointer();
		desc.CS.BytecodeLength = m_Shader_handle[m_Handle_id].shader->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = m_Shader_handle[m_Handle_id].root_sig;
		auto hr = m_Dx12->device()->CreateComputePipelineState(
			&desc,
			IID_PPV_ARGS(&m_Shader_handle[m_Handle_id].pipeline_state)
		);
	}
	void ComputeShader::createHeap(int slot_max) {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = slot_max;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto result = m_Dx12->device()->CreateDescriptorHeap(
			&desc, 
			IID_PPV_ARGS(&m_Shader_handle[m_Handle_id].desc_heap)
		);
	}
	void ComputeShader::inputBufferSize(int slot_id, int size, int type) {
		m_Shader_handle[m_Handle_id].element[slot_id] = { type,size };
	}

	void ComputeShader::createResource(bool is_alingnment, int slot_id) {
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		//サイズは定数バッファと同じように指定
		D3D12_RESOURCE_DESC desc =
			CD3DX12_RESOURCE_DESC::Buffer(
				m_Shader_handle[m_Handle_id].element[slot_id].num *
				m_Shader_handle[m_Handle_id].element[slot_id].size_of
			);
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		auto result = m_Dx12->device()->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&m_Shader_handle[m_Handle_id].resource[slot_id]));
	}
	void ComputeShader::createUnorderdAccessView() {
		auto handle = m_Shader_handle[m_Handle_id].desc_heap->GetCPUDescriptorHandleForHeapStart();
		for (int ite = 0; ite < m_Shader_handle[m_Handle_id].resource.size(); ite++) {
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.Buffer.NumElements = m_Shader_handle[m_Handle_id].element[ite].num;
			desc.Buffer.StructureByteStride = m_Shader_handle[m_Handle_id].element[ite].size_of;

			m_Dx12->device()->CreateUnorderedAccessView(m_Shader_handle[m_Handle_id].resource[ite], nullptr, &desc, handle);
			handle.ptr += m_Dx12->device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
	}
	void ComputeShader::mapInput(int slot_id, const void* data) {
		D3D12_RANGE range{ 0, 0 };
		UINT8* input_data;
		auto result = m_Shader_handle[m_Handle_id].resource[slot_id]->Map(0, &range, reinterpret_cast<void**>(&input_data));
		memcpy(input_data, data, 
			m_Shader_handle[m_Handle_id].element[slot_id].num * 
			m_Shader_handle[m_Handle_id].element[slot_id].size_of
		);
		//m_Shader_handle[m_Handle_id].resource[slot_id]->Unmap(0, nullptr);
	}
	void ComputeShader::mapOutput(int slot_id) {
		D3D12_RANGE range{ 0, 0 };
		auto result = m_Shader_handle[m_Handle_id].resource[slot_id]->Map(
			0, &range, &m_Shader_handle[m_Handle_id].data[slot_id]);
	}
	void ComputeShader::execution(int x, int y, int z) {
		m_Dx12->cmdList()->SetComputeRootSignature(m_Shader_handle[m_Handle_id].root_sig);
		m_Dx12->cmdList()->SetPipelineState(m_Shader_handle[m_Handle_id].pipeline_state);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &m_Shader_handle[m_Handle_id].desc_heap);
		auto handle = m_Shader_handle[m_Handle_id].desc_heap->GetGPUDescriptorHandleForHeapStart();
		m_Dx12->cmdList()->SetComputeRootDescriptorTable(0, handle);

		//コンピュートシェーダーの実行
		m_Dx12->cmdList()->Dispatch(x, y, z);
	}
	void* ComputeShader::getData(int slot_id) {
		return m_Shader_handle[m_Handle_id].data[slot_id];
	}
}