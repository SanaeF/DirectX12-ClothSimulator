#include "CSRootSignature.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
namespace phy {
	CSRootSignature::CSRootSignature(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		createDescriptorRange();
		createRootParameters();
		createSamplerDesc();
		createRSDesc();
		ID3DBlob* error_blob = nullptr;
		auto result = D3D12SerializeRootSignature(
			&m_RS_desc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&m_Root_sig,
			&error_blob
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "D3D12SerializeRootSignature Error!");
		}

		result = dx_12->device()->CreateRootSignature(
			0,
			m_Root_sig->GetBufferPointer(),
			m_Root_sig->GetBufferSize(),
			IID_PPV_ARGS(&m_Root_signature)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "CreateRootSignature Error!");
		}
	}
	void CSRootSignature::createDescriptorRange() {
		m_Desc_tbl_range[0].NumDescriptors = 1;
		m_Desc_tbl_range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		m_Desc_tbl_range[0].BaseShaderRegister = 0;
		m_Desc_tbl_range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		m_Desc_tbl_range[1].NumDescriptors = 1;
		m_Desc_tbl_range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		m_Desc_tbl_range[1].BaseShaderRegister = 0;
		m_Desc_tbl_range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
	void CSRootSignature::createRootParameters() {
		m_Root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_Root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_Root_parameter.DescriptorTable.pDescriptorRanges = m_Desc_tbl_range;
		m_Root_parameter.DescriptorTable.NumDescriptorRanges = 2;
	}
	void CSRootSignature::createSamplerDesc() {
		m_Sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_Sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_Sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_Sampler_desc.MipLODBias = 0.f;
		m_Sampler_desc.MaxAnisotropy = 0;
		m_Sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_Sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		m_Sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		m_Sampler_desc.MinLOD = 0.f;
		m_Sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
		m_Sampler_desc.RegisterSpace = 0;
		m_Sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		
	}
	void CSRootSignature::createRSDesc() {
		m_RS_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		m_RS_desc.pParameters = &m_Root_parameter;
		m_RS_desc.NumParameters = 1;
		m_RS_desc.pStaticSamplers = &m_Sampler_desc;
		m_RS_desc.NumStaticSamplers = 1;
	}
	ID3D12RootSignature*
		CSRootSignature::getResult() {
		return m_Root_signature;
	}
	ID3DBlob* CSRootSignature::getBlob() {
		return m_Root_sig;
	}
}