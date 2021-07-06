#pragma once
#include<d3dx12.h>
#include<wrl.h>
#include<memory>

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class Dx2DRootSignature {
	private:
		D3D12_ROOT_SIGNATURE_DESC m_Root_sigunature_desuc = {};
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		ComPtr<ID3DBlob> m_Root_sig_blob = nullptr;
		ComPtr<ID3DBlob> m_Error_blob = nullptr;
		ComPtr<ID3D12RootSignature> m_Root_signature = nullptr;
	public:
		ID3D12RootSignature* createRootSignature(lib::DirectX12Manager& DxWrap, D3D12_ROOT_SIGNATURE_DESC* mRootSigunatureDesuc);
		ID3D12RootSignature* getRootSignature();
	};
}