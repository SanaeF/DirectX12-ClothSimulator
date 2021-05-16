#pragma once
#include <d3d12.h>
#include <wrl.h>
namespace lib {
    class DirectX12Manager;
}
namespace compute {
	class ClothShader {
	private:
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;
        ID3D12RootSignature* root = nullptr;
        ID3DBlob* shader = nullptr;
        ID3D12PipelineState* pipe = nullptr;
        ID3D12DescriptorHeap* heap = nullptr;
        ID3D12Resource* rsc = nullptr;
        void* data = nullptr;
	public:
		ClothShader(ComPtr<ID3D12Device> device);
		~ClothShader();
        void ShaderCalculater(ComPtr<lib::DirectX12Manager>& dx12);
	private:
        long CreateRoot(ComPtr<ID3D12Device> device);// ルートシグネチャの生成
        long CreatePipe(ComPtr<ID3D12Device> device);// パイプラインの生成
        long CreateHeap(ComPtr<ID3D12Device> device);// ヒープの生成
        long CreateRsc(ComPtr<ID3D12Device> device);// リソースの生成
        void CreateUAV(ComPtr<ID3D12Device> device);// UAVの生成
        long Map();// リソースのマップ
	};
}