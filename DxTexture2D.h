#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>

class Dx12Wrapper;

class DxTexture2D {
private:

	std::shared_ptr<Dx12Wrapper> mDxWrap;

	D3D12_CPU_DESCRIPTOR_HANDLE mDescHandle;

	D3D12_HEAP_PROPERTIES mHeap_Prop = {};
	D3D12_HEAP_PROPERTIES mUploadHeap_Prop = {};

	D3D12_RESOURCE_DESC mResource_Desc = {};
	D3D12_DESCRIPTOR_HEAP_DESC mDescriptor_Heap = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC mSRV_Desc = {};
	D3D12_ROOT_PARAMETER mRoot_Parameter = {};

	CD3DX12_ROOT_PARAMETER mRoot_Params[2] = {};

	CD3DX12_ROOT_SIGNATURE_DESC mRootSigDesc = {};

	D3D12_ROOT_SIGNATURE_DESC mRS_Desc = {};
	D3D12_DESCRIPTOR_RANGE mDescTbl_Range[2] = {};
	D3D12_STATIC_SAMPLER_DESC mSampler_Desc = {};
	DirectX::TexMetadata mMetaData = {}; 
	DirectX::ScratchImage mScratch_Image = {};
	DirectX::Image mImg;

	ID3D12Resource* mTextureBuff = nullptr;

	ID3D12DescriptorHeap* mTexDescHeap = nullptr;
	ID3D12DescriptorHeap* mBasicDescHeap = nullptr;
public:

	void LoadWIC(const wchar_t* path);

	void CreateResource(std::shared_ptr<Dx12Wrapper> DxWrap);

	void mDescriptorHeap(std::shared_ptr<Dx12Wrapper> DxWrap, D3D12_DESCRIPTOR_HEAP_DESC DexcriptorHeap);

	void ShaderResourceView(
		std::shared_ptr<Dx12Wrapper> DxWrap,
		D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceView,
		ID3D12Resource* TextureBuffer,
		ID3D12DescriptorHeap* HeapHandle
	);

	void ConstBuffViwe(
		std::shared_ptr<Dx12Wrapper> DxWrap, 
		ID3D12Resource* constBuffer,
		D3D12_CPU_DESCRIPTOR_HANDLE DescHandle
	);

	void DescriptorHeap_Prop();

	void RootSignatureDesc_Prop();

	void ShaderResourceViewDesc_Prop();

	D3D12_DESCRIPTOR_HEAP_DESC getDescriptorHeap();
	D3D12_CPU_DESCRIPTOR_HANDLE getDescHandle();
	D3D12_SHADER_RESOURCE_VIEW_DESC getShaderResourceWierDesc();

	D3D12_ROOT_SIGNATURE_DESC* getRootSigDesc();

	ID3D12Resource* getTextureBuff();

	ID3D12DescriptorHeap* getTexDescHeap();

	ID3D12DescriptorHeap* getBasicDescHeap();

	DirectX::Image getImage();

	DirectX::TexMetadata getMetaData();

private:

	void mHeapProp();

	void mResourceDesc();

	void mGPUtransfer();

	void mDescriptorRange();

	void mRootParameters();


	void mSampler();

};