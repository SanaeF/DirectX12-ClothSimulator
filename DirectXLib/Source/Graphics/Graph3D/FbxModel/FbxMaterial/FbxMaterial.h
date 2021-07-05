#pragma once
#include <d3dx12.h>
#include <memory>
#include <wrl.h>
#include <fbxsdk.h>

namespace lib {
	class DirectX12Manager;
}
namespace model {
	class FbxMaterial {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::shared_ptr<lib::DirectX12Manager> mDx12;
		ComPtr<ID3D12DescriptorHeap> _materialHeap = nullptr;//マテリアルヒープ(5個ぶん)
		FbxLayerElementMaterial* m_Material;
	public:
		FbxMaterial(std::shared_ptr<lib::DirectX12Manager>& dx12);
		void loadMaterial(FbxMesh* mesh);
	private:
		void createBuffer();
		void heap();
	};
}