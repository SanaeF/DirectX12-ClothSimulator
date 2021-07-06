#pragma once
#include<vector>
#include<d3dx12.h>
#include<d3d12.h>
#include<DirectXMath.h>
#include<DirectXTex.h>

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class DxVertexProp {
	public:
		struct Vertex {
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 uv;
		};
	private:
		struct InstanceVertex {
			DirectX::XMFLOAT3 pos;
		};

		Vertex vertex[4] = {
	{{ -1.f,-1.f,0.f},{0.f,1.f}},
	{{ -1.f, 1.f,0.f},{0.f,0.f}},
	{{  1.f,-1.f,0.f},{1.f,1.f}},
	{{  1.f, 1.f,0.f},{1.f,0.f}}
		};

		//	Vertex vertex[4] = {
		//{{  0.f, 0.f,0.f},{0.f,1.f}},
		//{{  0.f, 0.f,0.f},{0.f,0.f}},
		//{{  0.f, 0.f,0.f},{1.f,1.f}},
		//{{  0.f, 0.f,0.f},{1.f,0.f}}
		//	};

		D3D12_HEAP_PROPERTIES mHeap_prop = {};

		D3D12_RESOURCE_DESC mResource_desc = {};

		D3D12_VERTEX_BUFFER_VIEW verBuffView = {};

		ID3D12Resource* mVertex_Buff = nullptr;

		Vertex* mVertexMap = nullptr;

	public:
		void mCreateBuff(lib::DirectX12Manager& DxWrap, std::vector<Vertex> vert);
		D3D12_VERTEX_BUFFER_VIEW getvertexBufferView();
		void setResourceDesc(D3D12_RESOURCE_DESC Resource_desc);
		void set2DPolygonSize(Vertex vert[4], SIZE size, DirectX::TexMetadata data);
		void changePol(Vertex vert[4], float x, float y);
		void Heap_Prop();
		D3D12_HEAP_PROPERTIES getHeap_prop();
		D3D12_RESOURCE_DESC getResource_desc();
	private:
		void mResourceDesc(std::vector<Vertex> vert);
		void mBufferCCR(lib::DirectX12Manager& DxWrap);
		void mCopyBuffer(std::vector<Vertex> vert);
		void mCretateBuffView(std::vector<Vertex> vert);
	};
}