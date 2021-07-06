#pragma once
#include "../DxGraphics2D/DxGraphics2DTexture/DxGraphics2DTexture.h"

namespace libGraph {
	class UploadTexture :public DxGraphics2DTexture {
	private:
		D3D12_HEAP_PROPERTIES m_Tex_heap_prop = {};
		D3D12_HEAP_PROPERTIES m_Upload_heap_prop = {};
		D3D12_RESOURCE_DESC m_Resource_desc = {};
		D3D12_RESOURCE_BARRIER m_Barrier_desc = {};
		D3D12_TEXTURE_COPY_LOCATION m_SRC = {};
		D3D12_TEXTURE_COPY_LOCATION m_DST = {};
		ID3D12DescriptorHeap* m_Texture_desc_heap = nullptr;
		ID3D12Resource* m_Upload_buff = nullptr;
		ID3D12Resource* m_Texture_buff = nullptr;
	public:
		void createTexBuffer(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		void resourceUpload();
		void textureCopy(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		void barrierDescProp(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		void shaderResourceViewforUpload(std::shared_ptr<lib::DirectX12Manager>& dx_12);

		ID3D12DescriptorHeap* getTexDescHeap();
		D3D12_RESOURCE_BARRIER getBarrier();
		D3D12_TEXTURE_COPY_LOCATION getSRC();
		D3D12_TEXTURE_COPY_LOCATION getDST();
	private:
		void uploadHeapProp();
		void uploadResourceDesc();
		void texHeapProp();
		void resourceProp();
		void createUploadResource(std::shared_ptr<lib::DirectX12Manager>& dx_12);
	};
}