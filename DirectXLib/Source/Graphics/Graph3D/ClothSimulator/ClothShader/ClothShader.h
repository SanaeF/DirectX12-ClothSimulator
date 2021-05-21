#pragma once
#include "../../Vertex/Vertex.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include<d3dx12.h>

namespace lib {
    class DirectX12Manager;
}
namespace compute {
	class ClothShader {
	private:
        struct ClothModel {
            std::vector<lib::Vertex> vertex;
            std::vector<lib::Vertex> perVertex;
            std::vector<UINT> index;
            int vertexSize;
            int indexSize;
        };
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;
        ID3D12RootSignature* root = nullptr;
        ID3DBlob* shader = nullptr;
        ID3D12PipelineState* pipe = nullptr;
        ID3D12DescriptorHeap* heap = nullptr;
        ID3D12Resource* rsc = nullptr;
        ClothModel modelData;
        void* data = nullptr;
	public:
        ClothShader(
            ComPtr<ID3D12Device> device, std::vector<lib::Vertex> vertex, std::vector<UINT> index
        );
		~ClothShader();
        void ShaderCalculater(std::shared_ptr<lib::DirectX12Manager> dx12);
        ClothModel getSimulatedData();
	private:
        long CreateRoot(ComPtr<ID3D12Device> device);// ルートシグネチャの生成
        long CreatePipe(ComPtr<ID3D12Device> device);// パイプラインの生成
        long CreateHeap(ComPtr<ID3D12Device> device);// ヒープの生成
        long CreateRsc(ComPtr<ID3D12Device> device);// リソースの生成
        void CreateUAV(ComPtr<ID3D12Device> device);// UAVの生成
        long Map();// リソースのマップ
	};
}