#include <comdef.h> 
#include "DxTexture2D.h"

#include"Dx12Wrapper.h"

void DxTexture2D::LoadWIC(const wchar_t* path) {
	auto result = DirectX::LoadFromWICFile(
		path,
		DirectX::WIC_FLAGS_NONE,
		&mMetaData,
		mScratch_Image
	);
	
	auto img = mScratch_Image.GetImage(0, 0, 0);
	mImg = *img;
}


void DxTexture2D::mHeapProp() {
	mHeap_Prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	mHeap_Prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	mHeap_Prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	mHeap_Prop.CreationNodeMask = 0;
	mHeap_Prop.VisibleNodeMask = 0;
}

void DxTexture2D::mResourceDesc() {
	mResource_Desc.Format = mMetaData.format;
	mResource_Desc.Width = mMetaData.width;
	mResource_Desc.Height = mMetaData.height;
	mResource_Desc.DepthOrArraySize = mMetaData.arraySize;
	mResource_Desc.SampleDesc.Count = 1;
	mResource_Desc.SampleDesc.Quality = 0;
	mResource_Desc.MipLevels = mMetaData.mipLevels;
	mResource_Desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(mMetaData.dimension);
	mResource_Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	mResource_Desc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

void DxTexture2D::CreateResource(std::shared_ptr<Dx12Wrapper> DxWrap) {
	mHeapProp();
	mResourceDesc();

	auto result = DxWrap->Device()->CreateCommittedResource(
		&mHeap_Prop,
		D3D12_HEAP_FLAG_NONE,
		&mResource_Desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&mTextureBuff)
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "CreateCommittedResource Error!");
	}
}

void DxTexture2D::GPUtransfer() {
	auto result = mTextureBuff->WriteToSubresource(
		0,
		nullptr,
		mImg.pixels,
		mImg.rowPitch,
		mImg.slicePitch
	);
}

void DxTexture2D::mDescriptorHeap(std::shared_ptr<Dx12Wrapper> DxWrap) {
	mDescriptor_Heap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	mDescriptor_Heap.NodeMask = 0;
	mDescriptor_Heap.NumDescriptors = 2;
	mDescriptor_Heap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto result = DxWrap->Device()->CreateDescriptorHeap(
		&mDescriptor_Heap,
		IID_PPV_ARGS(&mBasicDescHeap)
	);
}

void DxTexture2D::mDescriptorRange() {
	mDescTbl_Range[0].NumDescriptors = 1;
	mDescTbl_Range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	mDescTbl_Range[0].BaseShaderRegister = 0;
	mDescTbl_Range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	mDescTbl_Range[1].NumDescriptors = 1;
	mDescTbl_Range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	mDescTbl_Range[1].BaseShaderRegister = 0;
	mDescTbl_Range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void DxTexture2D::mRootParameters() {
	mRoot_Parameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	mRoot_Parameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	mRoot_Parameter[0].DescriptorTable.pDescriptorRanges = &mDescTbl_Range[0];
	mRoot_Parameter[0].DescriptorTable.NumDescriptorRanges = 1;

	mRoot_Parameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	mRoot_Parameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	mRoot_Parameter[1].DescriptorTable.pDescriptorRanges = &mDescTbl_Range[1];
	mRoot_Parameter[1].DescriptorTable.NumDescriptorRanges = 1;
}

void DxTexture2D::mSampler() {
	mSampler_Desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	mSampler_Desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	mSampler_Desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	mSampler_Desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	mSampler_Desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	mSampler_Desc.MaxLOD = D3D12_FLOAT32_MAX;
	mSampler_Desc.MinLOD = 0.0f;
	mSampler_Desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	mSampler_Desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
}

void DxTexture2D::mRootSignatureDesc() {
	mRS_Desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	mRS_Desc.pParameters = mRoot_Parameter;
	mRS_Desc.NumParameters = 2;
	mRS_Desc.pStaticSamplers = &mSampler_Desc;
	mRS_Desc.NumStaticSamplers = 1;
}

void DxTexture2D::ShaderResourceView(std::shared_ptr<Dx12Wrapper> DxWrap, ID3D12Resource* constBuffer) {
	mDescriptorHeap(DxWrap);
	mSRV_Desc.Format = mMetaData.format;
	mSRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSRV_Desc.Texture2D.MipLevels = 1;

	auto basicHeapHandle = mBasicDescHeap->GetCPUDescriptorHandleForHeapStart();

	DxWrap->Device()->CreateShaderResourceView(
		mTextureBuff,
		&mSRV_Desc,
		basicHeapHandle
	);
	basicHeapHandle.ptr += DxWrap->Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuffer->GetDesc().Width;
	DxWrap->Device()->CreateConstantBufferView(&cbvDesc,basicHeapHandle);

	mDescriptorRange();
	mRootParameters();
	mSampler();
	mRootSignatureDesc();
}

D3D12_ROOT_SIGNATURE_DESC* 
DxTexture2D::getRootSigDesc() {
	return &mRS_Desc;
}

ID3D12DescriptorHeap* 
DxTexture2D::getTexDescHeap() {
	return mTexDescHeap;
}

ID3D12DescriptorHeap*
DxTexture2D::getBasicDescHeap() {
	return mBasicDescHeap;
}