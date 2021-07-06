#pragma once
#include<d3dx12.h>
#include<DirectXTex.h>

namespace lib {
	class DirectX12Manager;
}
class DxGraphics2DTexture {
private:
	std::shared_ptr<lib::DirectX12Manager> m_Dx12;
	D3D12_CPU_DESCRIPTOR_HANDLE m_Desc_handle;
	D3D12_HEAP_PROPERTIES m_Heap_prop = {};
	D3D12_HEAP_PROPERTIES m_Upload_heap_prop = {};
	D3D12_RESOURCE_DESC m_Resource_desc = {};
	D3D12_DESCRIPTOR_HEAP_DESC m_Descriptor_heap = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC m_SRV_desc = {};
	D3D12_ROOT_PARAMETER m_Root_parameter = {};
	CD3DX12_ROOT_PARAMETER m_Root_params[2] = {};
	CD3DX12_ROOT_SIGNATURE_DESC m_Root_sig_desc = {};
	D3D12_ROOT_SIGNATURE_DESC m_RS_desc = {};
	D3D12_DESCRIPTOR_RANGE m_Desc_tbl_range[2] = {};
	D3D12_STATIC_SAMPLER_DESC m_Sampler_desc = {};
	DirectX::TexMetadata m_Meta_data = {}; 
	DirectX::ScratchImage m_Scratch_img = {};
	DirectX::Image m_Img;
	ID3D12Resource* m_Tex_buff = nullptr;
	ID3D12DescriptorHeap* m_Tex_desc_heap = nullptr;
	ID3D12DescriptorHeap* m_Basic_desc_heap = nullptr;
public:
	void loadWIC(const wchar_t* path);
	void createResource(std::shared_ptr<lib::DirectX12Manager>& dx_12);
	void descriptorHeap(std::shared_ptr<lib::DirectX12Manager>& dx_12, D3D12_DESCRIPTOR_HEAP_DESC DexcriptorHeap);
	void shaderResourceView(
		std::shared_ptr<lib::DirectX12Manager>& dx_12,
		D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceView,
		ID3D12Resource* TextureBuffer,
		ID3D12DescriptorHeap* HeapHandle
	);
	void constBuffViwe(
		std::shared_ptr<lib::DirectX12Manager>& dx_12,
		ID3D12Resource* constBuffer,
		D3D12_CPU_DESCRIPTOR_HANDLE DescHandle
	);
	void descriptorHeapProp();
	void rootSignatureDescProp();
	void shaderResourceViewDescProp();
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
	void heapProp();
	void resourceDesc();
	void transferGPU();
	void descriptorRange();
	void rootParameters();
	void sampler();
};