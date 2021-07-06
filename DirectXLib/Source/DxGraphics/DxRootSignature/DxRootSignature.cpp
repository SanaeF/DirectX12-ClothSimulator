#include "DxRootSignature.h"
#include <cassert>
#include "../../DirectX12Manager/DirectX12Manager.h"

namespace libGraph {
	ID3D12RootSignature* Dx2DRootSignature::createRootSignature(lib::DirectX12Manager& DxWrap, D3D12_ROOT_SIGNATURE_DESC* mRootSigunatureDesuc) {
		//mRootSigBlob->Release();
		auto result = D3D12SerializeRootSignature(
			mRootSigunatureDesuc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&m_Root_sig_blob,
			&m_Error_blob
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "D3D12SerializeRootSignature Error!");
		}

		result = DxWrap.device()->CreateRootSignature(
			0,
			m_Root_sig_blob->GetBufferPointer(),
			m_Root_sig_blob->GetBufferSize(),
			IID_PPV_ARGS(&m_Root_signature)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "CreateRootSignature Error!");
		}

		return m_Root_signature.Get();
	}

	ID3D12RootSignature* Dx2DRootSignature::getRootSignature() {
		return m_Root_signature.Get();
	}
}