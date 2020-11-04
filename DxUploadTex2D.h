#pragma once
#include "DxTexture2D.h"

class DxUploadTex2D :public DxTexture2D {
private:

	D3D12_HEAP_PROPERTIES mTexHeap_Prop = {};
	D3D12_HEAP_PROPERTIES mUploadHeap_Prop = {};
	D3D12_RESOURCE_DESC mResource_Desc = {};
	D3D12_RESOURCE_BARRIER mBarrier_Desc = {};
	D3D12_TEXTURE_COPY_LOCATION mSRC = {};
	D3D12_TEXTURE_COPY_LOCATION mDST = {};

	ID3D12DescriptorHeap* mTexture_DescHeap = nullptr;

	ID3D12Resource* mUploadBuff = nullptr;
	ID3D12Resource* mTextureBuff = nullptr;

public:

	void CreateTexBuffer(std::shared_ptr<Dx12Wrapper> DxWrap);

	void ResourceUpload();

	void TextureCopy(std::shared_ptr<Dx12Wrapper> DxWrap);

	void BarrierDescProp(std::shared_ptr<Dx12Wrapper> DxWrap);

	void ShaderResourceViewforUpload(std::shared_ptr<Dx12Wrapper> DxWrap);

	ID3D12DescriptorHeap* getTexDescHeap();

	D3D12_RESOURCE_BARRIER getBarrier();
	D3D12_TEXTURE_COPY_LOCATION getSRC();
	D3D12_TEXTURE_COPY_LOCATION getDST();

private:

	void mUploadHeapProp();

	void mUploadResourceDesc();

	void mTexHeapProp();

	void mResourceProp();

	void mCreateUploadResource(std::shared_ptr<Dx12Wrapper> DxWrap);

};