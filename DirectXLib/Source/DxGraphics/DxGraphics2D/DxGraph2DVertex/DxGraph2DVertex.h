#pragma once
#include<d3dx12.h>
#include<d3d12.h>
#include<DirectXMath.h>

#include<DirectXTex.h>

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxGraph2DVertex {
	private:
		struct Vertex {
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 uv;
		};
		struct InstanceVertex {
			DirectX::XMFLOAT3 pos;
		};
		Vertex m_Vertex[4] = {
			{{ -1.f,-1.f,0.f},{0.f,1.f}},
			{{ -1.f, 1.f,0.f},{0.f,0.f}},
			{{  1.f,-1.f,0.f},{1.f,1.f}},
			{{  1.f, 1.f,0.f},{1.f,0.f}}
		};
		D3D12_HEAP_PROPERTIES m_Heap_prop = {};

		D3D12_RESOURCE_DESC m_Resource_desc = {};

		D3D12_VERTEX_BUFFER_VIEW m_VB_view = {};

		ID3D12Resource* m_Vertex_buff = nullptr;

		Vertex* m_pVertex_map = nullptr;

	public:
		void createBuff(lib::DirectX12Manager& dx_12);
		void cretateBuffView();
		D3D12_VERTEX_BUFFER_VIEW getVertexBufferView();
		void setResourceDesc(D3D12_RESOURCE_DESC Resource_desc);
		void setPolygonSize(SIZE size, DirectX::TexMetadata data);
		void changePolygon(float x, float y);
		void heapProp();
		D3D12_HEAP_PROPERTIES getHeapProp();
		D3D12_RESOURCE_DESC getResourceDesc();
	private:
		void resourceDesc();
		void commitBuffer(lib::DirectX12Manager& dx_12);
		void copyBuffer();
	};
}