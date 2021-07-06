#include "UploadTexture.h"
#include "../../LibHelper/LibHelper.h"

#include"../../DirectX12Manager/DirectX12Manager.h"

namespace libGraph {
	void UploadTexture::uploadHeapProp() {
		m_Upload_heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		m_Upload_heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		m_Upload_heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		m_Upload_heap_prop.CreationNodeMask = 0;
		m_Upload_heap_prop.VisibleNodeMask = 0;
	}

	void UploadTexture::uploadResourceDesc() {
		m_Resource_desc.Format = DXGI_FORMAT_UNKNOWN;
		m_Resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		m_Resource_desc.Width = getImage().slicePitch;
		m_Resource_desc.Height = 1;
		m_Resource_desc.DepthOrArraySize = 1;
		m_Resource_desc.MipLevels = 1;
		m_Resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		m_Resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		m_Resource_desc.SampleDesc.Count = 1;
		m_Resource_desc.SampleDesc.Quality = 0;
	}

	void UploadTexture::createUploadResource(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		this->uploadHeapProp();
		this->uploadResourceDesc();

		auto result = dx_12->device()->CreateCommittedResource(
			&m_Upload_heap_prop,
			D3D12_HEAP_FLAG_NONE,
			&m_Resource_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_Upload_buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "UploadBuff Error!");
		}
	}

	void UploadTexture::texHeapProp() {
		m_Tex_heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		m_Tex_heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		m_Tex_heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		m_Tex_heap_prop.CreationNodeMask = 0;
		m_Tex_heap_prop.VisibleNodeMask = 0;
	}

	void UploadTexture::resourceProp() {
		m_Resource_desc.Format = getMetaData().format;
		m_Resource_desc.Width = getMetaData().width;
		m_Resource_desc.Height = getMetaData().height;
		m_Resource_desc.DepthOrArraySize = getMetaData().arraySize;
		m_Resource_desc.MipLevels = getMetaData().mipLevels;
		m_Resource_desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(getMetaData().dimension);
		m_Resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	}


	void UploadTexture::createTexBuffer(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		createUploadResource(dx_12);
		texHeapProp();
		resourceProp();

		auto result = dx_12->device()->CreateCommittedResource(
			&m_Tex_heap_prop,
			D3D12_HEAP_FLAG_NONE,
			&m_Resource_desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_Texture_buff)
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "TextureBuff Error!");
		}
	}

	void UploadTexture::resourceUpload() {
		helper::LibHelper Helper;
		uint8_t* mMap_for_Image = nullptr;
		//auto srcAddress = getImage().pixels;
		//auto rowPitch = 
		//	Helper.AlignmentedSize(
		//	getImage().rowPitch,
		//	D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
		//);
		//for (int y = 0; y < getImage().height; y++) {
		//	std::copy_n(//
		//		srcAddress,
		//		rowPitch,
		//		mMap_for_Image
		//	);
		//	srcAddress += getImage().rowPitch;
		//	mMap_for_Image += rowPitch;
		//}
		auto result = m_Upload_buff->Map(0, nullptr, (void**)&mMap_for_Image);
		std::copy_n(getImage().pixels, getImage().slicePitch, mMap_for_Image);
		m_Upload_buff->Unmap(0, nullptr);
	}

	void UploadTexture::textureCopy(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		helper::LibHelper Helper;
		m_SRC.pResource = m_Upload_buff;
		m_SRC.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
		UINT nrow;
		UINT64 rowsize, size;
		auto desc = m_Texture_buff->GetDesc();
		dx_12->device()->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
		m_SRC.PlacedFootprint = footprint;
		m_SRC.PlacedFootprint.Offset = 0;
		//mSRC.PlacedFootprint.Footprint.Width = getMetaData().width;

		m_SRC.PlacedFootprint.Footprint.Width =
			Helper.alignmentedSize(
				getImage().rowPitch,
				D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
			) * getImage().height;

		m_SRC.PlacedFootprint.Footprint.Height = getMetaData().height;
		m_SRC.PlacedFootprint.Footprint.Depth = getMetaData().depth;
		//mSRC.PlacedFootprint.Footprint.RowPitch = getImage().rowPitch;

		m_SRC.PlacedFootprint.Footprint.RowPitch =
			Helper.alignmentedSize(
				getImage().rowPitch,
				D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
			);

		m_SRC.PlacedFootprint.Footprint.Format = getImage().format;
		m_DST.pResource = m_Texture_buff;
		m_DST.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		m_DST.SubresourceIndex = 0;
	}

	void UploadTexture::barrierDescProp(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		dx_12->cmdList()->CopyTextureRegion(
			&m_DST, 0, 0, 0, &m_SRC, nullptr
		);

		m_Barrier_desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		m_Barrier_desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		m_Barrier_desc.Transition.pResource = m_Texture_buff;
		m_Barrier_desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_Barrier_desc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		m_Barrier_desc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		dx_12->cmdList()->ResourceBarrier(1, &m_Barrier_desc);
		dx_12->cmdList()->Close();
		auto result = dx_12->device()->GetDeviceRemovedReason();
		if (SUCCEEDED(result))assert(0);
	}

	void UploadTexture::shaderResourceViewforUpload(std::shared_ptr<lib::DirectX12Manager>& dx_12) {
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
		descHeapDesc.NodeMask = 0;//マスクは0
		descHeapDesc.NumDescriptors = 1;//ビューは今のところ１つだけ
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//シェーダリソースビュー(および定数、UAVも)

		auto result = dx_12->device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_Texture_desc_heap));//生成

		//通常テクスチャビュー作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = getMetaData().format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f～1.0fに正規化)
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1

		dx_12->device()->CreateShaderResourceView(
			m_Texture_buff, //ビューと関連付けるバッファ
			&srvDesc, //先ほど設定したテクスチャ設定情報
			m_Texture_desc_heap->GetCPUDescriptorHandleForHeapStart()//ヒープのどこに割り当てるか
		);
	}

	ID3D12DescriptorHeap*
		UploadTexture::getTexDescHeap() {
		return m_Texture_desc_heap;
	}
	D3D12_RESOURCE_BARRIER
		UploadTexture::getBarrier() {
		return m_Barrier_desc;
	}
	D3D12_TEXTURE_COPY_LOCATION
		UploadTexture::getSRC() {
		return m_SRC;
	}
	D3D12_TEXTURE_COPY_LOCATION
		UploadTexture::getDST() {
		return m_DST;
	}
}