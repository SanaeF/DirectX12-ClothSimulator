#pragma once
#include <vector>
#include<d3dx12.h>
#include <wrl.h>
#include "../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class ComputeShader {
	private:
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;

        ID3DBlob* m_Blob = nullptr;
        ID3D12RootSignature* m_Root_sig = nullptr;
        ID3D12PipelineState* m_pipeline = nullptr;
        ID3D12Resource* m_Buffers_on_gpu[2] = { nullptr };
        void* m_Buffers_on_cpu[2] = { nullptr };
    public:
        ComputeShader(ComPtr<ID3D12Device> device, const char* file_path, const char* func_name);
        void setInputData(
            ComPtr<ID3D12Device> device,
            int sizeOfElement,
            int numElement,
            void* initData
        );
        void setOutputData(
            ComPtr<ID3D12Device> device,
            int sizeOfElement,
            int numElement,
            void* initData
        );
        void commitData();
    private:
        void load(const char* file_path, const char* func_name);
        void rootSignature(ComPtr<ID3D12Device> device);
        void piplineState(ComPtr<ID3D12Device> device);
	};
}