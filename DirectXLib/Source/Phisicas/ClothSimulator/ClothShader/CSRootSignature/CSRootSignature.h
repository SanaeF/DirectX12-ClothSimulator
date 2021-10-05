#pragma once
#include<d3dx12.h>
#include<memory>
namespace lib {
	class DirectX12Manager;
}
namespace phy {
	class CSRootSignature {
	private:
		ID3DBlob* m_Root_sig = nullptr;
		D3D12_DESCRIPTOR_RANGE m_Desc_tbl_range[2] = {};
		D3D12_ROOT_PARAMETER m_Root_parameter = {};
		D3D12_STATIC_SAMPLER_DESC m_Sampler_desc = {};
		D3D12_ROOT_SIGNATURE_DESC m_RS_desc = {};
		ID3D12RootSignature* m_Root_signature = nullptr;
	public:
		CSRootSignature(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		ID3D12RootSignature* getResult();
		ID3DBlob* getBlob();
	private:
		void createDescriptorRange();
		void createRootParameters();
		void createSamplerDesc();
		void createRSDesc();
	};
}