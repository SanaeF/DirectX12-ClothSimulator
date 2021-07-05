#include "ComputeShader.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
namespace lib {
	ComputeShader::ComputeShader(
		ComPtr<ID3D12Device> device, const char* file_path, const char* func_name
	) {
		load(file_path, func_name);
		rootSignature(device);
		piplineState(device);
	}
	inline void ComputeShader::load(const char* file_path, const char* func_name) {
		wchar_t wfxFilePath[256] = { L"" };
		mbstowcs(wfxFilePath, file_path, 256);
		auto result = D3DCompileFromFile(wfxFilePath, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, func_name, "cs_5_1", 
			D3DCOMPILE_DEBUG |D3DCOMPILE_SKIP_OPTIMIZATION, 
			0, &m_Blob, nullptr);
	}
	inline void ComputeShader::rootSignature(ComPtr<ID3D12Device> device) {
		ID3DBlob* sig = nullptr;
		auto result = D3DGetBlobPart(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		result = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
			IID_PPV_ARGS(&m_Root_sig));
	}
	inline void ComputeShader::piplineState(ComPtr<ID3D12Device> device) {
		D3D12_COMPUTE_PIPELINE_STATE_DESC  psoDesc = { 0 };
		psoDesc.pRootSignature = m_Root_sig;
		psoDesc.CS = CD3DX12_SHADER_BYTECODE(m_Blob);
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		psoDesc.NodeMask = 0;
		auto result = device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	}
	void ComputeShader::setInputData(
		ComPtr<ID3D12Device> device,
		int sizeOfElement,
		int numElement,
		void* initData
	) {
		auto m_sizeOfElement = sizeOfElement;
		auto m_numElement = numElement;

		int bufferNo = 0;
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto rDesc = CD3DX12_RESOURCE_DESC::Buffer(m_sizeOfElement * m_numElement);
		for (auto& buffer : m_Buffers_on_gpu) {
			auto hr = device->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&rDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&buffer)
			);


			//構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
			//マップ、アンマップのオーバーヘッドを軽減するためにはこのインスタンスが生きている間は行わない。
			{
				CD3DX12_RANGE readRange(0, 0);        //     intend to read from this resource on the CPU.
				buffer->Map(0, &readRange, reinterpret_cast<void**>(&m_Buffers_on_cpu[bufferNo]));
			}
			if (initData != nullptr) {
				memcpy(m_Buffers_on_cpu[bufferNo], initData, m_sizeOfElement * m_numElement);
			}

			bufferNo++;
		}
		//m_isInited = true;
	}
	void ComputeShader::setOutputData(
		ComPtr<ID3D12Device> device,
		int sizeOfElement,
		int numElement,
		void* initData
	) {
		auto m_sizeOfElement = sizeOfElement;
		auto m_numElement = numElement;
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(m_sizeOfElement * m_numElement);
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		int bufferNo = 0;

		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;

		for (auto& buffer : m_Buffers_on_gpu) {
			device->CreateCommittedResource(
				&prop,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
				nullptr,
				IID_PPV_ARGS(&buffer)
			);
			//構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
			//マップ、アンマップのオーバーヘッドを軽減するためにはこのインスタンスが生きている間は行わない。
			{
				CD3DX12_RANGE readRange(0, 0);        //     intend to read from this resource on the CPU.
				buffer->Map(0, &readRange, reinterpret_cast<void**>(&m_Buffers_on_cpu[bufferNo]));
			}
			if (initData != nullptr) {
				memcpy(m_Buffers_on_cpu[bufferNo], initData, m_sizeOfElement * m_numElement);
			}
			bufferNo++;
		}
	}
	void ComputeShader::commitData() {

	}
}