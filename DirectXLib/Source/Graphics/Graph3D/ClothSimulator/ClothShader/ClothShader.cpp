#include "ClothShader.h"
#include <iostream>
#include "../../DirectX12/DirectXLib/Source/DirectX12/DirectX12Manager.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace compute {
    std::vector<float>test(256, 0);
    ClothShader::ClothShader(ComPtr<ID3D12Device> device) {
        CreateRoot(device);
        CreatePipe(device);
        CreateHeap(device);
        CreateRsc(device);
        CreateUAV(device);
        Map();
    }
    void ClothShader::ShaderCalculater(ComPtr<lib::DirectX12Manager>& dx12) {

        dx12->CmdList()->SetComputeRootSignature(root);
        dx12->CmdList()->SetPipelineState(pipe);
        dx12->CmdList()->SetDescriptorHeaps(1, &heap);
        auto handle = heap->GetGPUDescriptorHandleForHeapStart();
        dx12->CmdList()->SetComputeRootDescriptorTable(0, handle);

        //コンピュートシェーダーの実行(今回は256個のスレッドグループを指定)
        dx12->CmdList()->Dispatch(test.size(), 1, 1);

        dx12->CmdList()->Close();

        test.assign((float*)data, (float*)data + test.size());
    }

    long ClothShader::CreateRoot(ComPtr<ID3D12Device> device) {//シェーダーコンパイル
        auto result = D3DCompileFromFile(L"ClothPhisical.hlsl", nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE, "ClothPhisical", "cs_5_1", D3DCOMPILE_DEBUG |
            D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
        ID3DBlob* sig = nullptr;
        result = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(),
            D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
        result = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
            IID_PPV_ARGS(&root));
        return result;
    }
    long ClothShader::CreatePipe(ComPtr<ID3D12Device> device) {
        D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
        desc.CS.pShaderBytecode = shader->GetBufferPointer();
        desc.CS.BytecodeLength = shader->GetBufferSize();
        desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
        desc.NodeMask = 0;
        desc.pRootSignature = root;
        auto result = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
        return result;
    }
    long ClothShader::CreateHeap(ComPtr<ID3D12Device> device) {//テストとしてリソースは1つとして作成
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask = 0;
        desc.NumDescriptors = 1;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        auto result = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
        return result;
    }
    long ClothShader::CreateRsc(ComPtr<ID3D12Device> device) {
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

        auto result = device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
            IID_PPV_ARGS(&rsc));
            return result;
    }
    void ClothShader::CreateUAV(ComPtr<ID3D12Device> device) {//GPUから受け取るのはchar型にしています
        D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
        desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
        desc.Format = DXGI_FORMAT_UNKNOWN;//char?
        desc.Buffer.NumElements = test.size();
        desc.Buffer.StructureByteStride = sizeof(float);

        device->CreateUnorderedAccessView(rsc, nullptr, &desc, heap->GetCPUDescriptorHandleForHeapStart());
    }
    long ClothShader::Map() {
        D3D12_RANGE range{ 0, 1 };
        auto result = rsc->Map(0, &range, &data);
        return result;
    }
    ClothShader::~ClothShader() {

    }
}