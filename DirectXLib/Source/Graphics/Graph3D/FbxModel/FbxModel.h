#pragma once
#include <d3dx12.h>
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")

namespace model {
	class FbxModel {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		struct Vertex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 color;
		};
		Vertex* m_Vertices;
		std::vector<Vertex>mVertex;
		std::vector<UINT>mIndex;
		FbxManager* m_Manager = nullptr;
		FbxScene* m_Scene = nullptr;
		FbxMesh* m_Mesh = nullptr;
		ID3D12Resource* m_vertexBuffer = nullptr;
		ID3D12Resource* m_indexBuffer = nullptr;
		CD3DX12_RANGE readRange;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		int m_VertexNum;
		int m_IndexNum;
	public:
		FbxModel(const char* FileName);
		~FbxModel();
		void createViewBuffer(ComPtr<ID3D12Device> device);
		int getVertexNum();
		int getIndexNum();
		ID3D12Resource* getVertexBuffer();
		ID3D12Resource* getIndexBuffer();
		D3D12_VERTEX_BUFFER_VIEW getVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW getIndexBufferView();
		Vertex* getVertex();
	private:
		void mLoadFile(const char* FileName);
		bool mDisplayContent(FbxScene* scene);
		bool mDisplayContent(FbxNode* node);
		void mLoadMesh(FbxNode* pNode);
		bool mLoadIndex(FbxMesh* mesh);
		bool mLoadVertex(FbxMesh* mesh);
		bool mCreateVertexBuffer(ComPtr<ID3D12Device> device);
		bool mCreateIndexBuffer(ComPtr<ID3D12Device> device);
	};
}