#pragma once
#include<d3dx12.h>
#include<wrl.h>
#include<memory>

class Dx12Wrapper;

class Dx2DRootSignature {
private:

	D3D12_ROOT_SIGNATURE_DESC mRootSigunatureDesuc = {};

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> mRootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;



public:

	ID3D12RootSignature* CreateRootSignature(Dx12Wrapper& DxWrap, D3D12_ROOT_SIGNATURE_DESC* mRootSigunatureDesuc);

	ID3D12RootSignature* getRootSignature();

};