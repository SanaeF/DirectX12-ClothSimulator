#include "DxGraphics2DTexture.h"
#include <comdef.h> 
#include <cassert>

#include"../../../DirectX12Manager/DirectX12Manager.h"

void DxGraphics2DTexture::loadWIC(const wchar_t* path) {
	auto result = DirectX::LoadFromWICFile(
		path,
		DirectX::WIC_FLAGS_NONE,
		&m_Meta_data,
		m_Scratch_img
	);
	auto img = m_Scratch_img.GetImage(0, 0, 0);
	if (img == nullptr)assert(0,"âÊëúÇ™Ç†ÇËÇ‹ÇπÇÒ(ÉçÅ[ÉhÇ…é∏îs)");
	m_Img = *img;
}

void DxGraphics2DTexture::heapProp() {
	m_Heap_prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	m_Heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	m_Heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	m_Heap_prop.CreationNodeMask = 0;
	m_Heap_prop.VisibleNodeMask = 0;
}

void DxGraphics2DTexture::resourceDesc() {
	m_Resource_desc.Format = m_Meta_data.format;
	m_Resource_desc.Width = m_Meta_data.width;
	m_Resource_desc.Height = m_Meta_data.height;
	m_Resource_desc.DepthOrArraySize = m_Meta_data.arraySize;
	m_Resource_desc.SampleDesc.Count = 1;
	m_Resource_desc.SampleDesc.Quality = 0;
	m_Resource_desc.MipLevels = m_Meta_data.mipLevels;
	m_Resource_desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(m_Meta_data.dimension);
	m_Resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	m_Resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

void DxGraphics2DTexture::transferGPU() {
	auto result = m_Tex_buff->WriteToSubresource(
		0,
		nullptr,
		m_Img.pixels,
		m_Img.rowPitch,
		m_Img.slicePitch
	);
}

void DxGraphics2DTexture::createResource(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
	heapProp();
	resourceDesc();

	auto result = dx_12->device()->CreateCommittedResource(
		&m_Heap_prop,
		D3D12_HEAP_FLAG_NONE,
		&m_Resource_desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&m_Tex_buff)
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "CreateCommittedResource Error!");
	}
	transferGPU();
}

void DxGraphics2DTexture::descriptorHeapProp() {
	m_Descriptor_heap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_Descriptor_heap.NodeMask = 0;
	m_Descriptor_heap.NumDescriptors = 2;
	m_Descriptor_heap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}

void DxGraphics2DTexture::descriptorHeap(
	std::shared_ptr<lib::DirectX12Manager>& DxWrap,
	D3D12_DESCRIPTOR_HEAP_DESC DexcriptorHeap
) {
	auto result = DxWrap->device()->CreateDescriptorHeap(
		&DexcriptorHeap,
		IID_PPV_ARGS(&m_Basic_desc_heap)
	);

	if (!SUCCEEDED(result)) {
		assert(0 && "BasicDescHeap Error!");
	}
}


void DxGraphics2DTexture::shaderResourceViewDescProp() {
	m_SRV_desc.Format = m_Meta_data.format;
	m_SRV_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_SRV_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_SRV_desc.Texture2D.MipLevels = 1;
}

void DxGraphics2DTexture::shaderResourceView(
	std::shared_ptr<lib::DirectX12Manager>& dx_12,
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceView, 
	ID3D12Resource* TextureBuffer,
	ID3D12DescriptorHeap* HeapHandle
) {

	auto basicHeapHandle = HeapHandle->GetCPUDescriptorHandleForHeapStart();

	dx_12->device()->CreateShaderResourceView(
		TextureBuffer,
		&ShaderResourceView,
		basicHeapHandle
	);
	basicHeapHandle.ptr += dx_12->device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_Desc_handle = basicHeapHandle;
}


void DxGraphics2DTexture::descriptorRange() {
	m_Desc_tbl_range[0].NumDescriptors = 1;
	m_Desc_tbl_range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	m_Desc_tbl_range[0].BaseShaderRegister = 0;
	m_Desc_tbl_range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	m_Desc_tbl_range[1].NumDescriptors = 1;
	m_Desc_tbl_range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	m_Desc_tbl_range[1].BaseShaderRegister = 0;
	m_Desc_tbl_range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	m_Desc_tbl_range[2].NumDescriptors = 1;
	m_Desc_tbl_range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	m_Desc_tbl_range[2].BaseShaderRegister = 1;
	m_Desc_tbl_range[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void DxGraphics2DTexture::rootParameters() {
	descriptorRange();
	m_Root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	m_Root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_Root_parameter.DescriptorTable.pDescriptorRanges = m_Desc_tbl_range;
	m_Root_parameter.DescriptorTable.NumDescriptorRanges = 3;

}

void DxGraphics2DTexture::sampler() {
	m_Sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_Sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_Sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_Sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	m_Sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	m_Sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
	m_Sampler_desc.MinLOD = 0.0f;
	m_Sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_Sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
}

void DxGraphics2DTexture::rootSignatureDescProp() {
	rootParameters();
	sampler();
	m_RS_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	m_RS_desc.pParameters = &m_Root_parameter;
	m_RS_desc.NumParameters = 1;
	m_RS_desc.pStaticSamplers = &m_Sampler_desc;
	m_RS_desc.NumStaticSamplers = 1;

}

void DxGraphics2DTexture::constBuffViwe(
	std::shared_ptr<lib::DirectX12Manager>& dx_12,
	ID3D12Resource* constBuffer,
	D3D12_CPU_DESCRIPTOR_HANDLE DescHandle
) {
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuffer->GetDesc().Width;
	dx_12->device()->CreateConstantBufferView(&cbvDesc, DescHandle);
	constBuffer->Unmap(0, nullptr);
}

D3D12_DESCRIPTOR_HEAP_DESC 
DxGraphics2DTexture::getDescriptorHeap() {
	return m_Descriptor_heap;
}

D3D12_CPU_DESCRIPTOR_HANDLE 
DxGraphics2DTexture::getDescHandle() {
	return m_Desc_handle;
}

D3D12_SHADER_RESOURCE_VIEW_DESC 
DxGraphics2DTexture::getShaderResourceWierDesc() {
	return m_SRV_desc;
}

ID3D12Resource* 
DxGraphics2DTexture::getTextureBuff() {
	return m_Tex_buff;
}

D3D12_ROOT_SIGNATURE_DESC*
DxGraphics2DTexture::getRootSigDesc() {
	return &m_RS_desc;
}
ID3D12DescriptorHeap* 
DxGraphics2DTexture::getTexDescHeap() {
	return m_Tex_desc_heap;
}
ID3D12DescriptorHeap*
DxGraphics2DTexture::getBasicDescHeap() {
	return m_Basic_desc_heap;
}
DirectX::Image
DxGraphics2DTexture::getImage() {
	return m_Img;
}

DirectX::TexMetadata 
DxGraphics2DTexture::getMetaData() {
	return m_Meta_data;
}