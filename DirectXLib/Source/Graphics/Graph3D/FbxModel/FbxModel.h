#pragma once
#include "../../../../../DirectXLib/Source/Graphics/Graph3D/Vertex/Vertex.h"
#include <d3dx12.h>
#include <fbxsdk.h>
#include <vector>
#include <memory>
#include <wrl.h>
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")

namespace lib {
	class DirectX12Manager;
	class ClothSimulator;
}
namespace model {
	class FbxModel {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::vector<lib::Vertex>mVertex;
		std::vector<UINT>mIndex;
		FbxManager* m_Manager = nullptr;
		FbxScene* m_Scene = nullptr;
		FbxMesh* m_Mesh = nullptr;
		ID3D12Resource* m_vertexBuffer = nullptr;
		ID3D12Resource* m_indexBuffer = nullptr;
		CD3DX12_RANGE readRange;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		std::shared_ptr<lib::ClothSimulator> mClothSim;
		int m_VertexNum;
		int m_IndexNum;
	public:
		FbxModel();
		~FbxModel();
		void load(const char* FileName);
		void setClothSimulator(ComPtr<ID3D12Device> device);
		void createViewBuffer(ComPtr<ID3D12Device> device);

		void calculatePhysics(
			std::shared_ptr<lib::DirectX12Manager> dx12,
			std::vector<lib::Vertex>& vertex,
			std::vector<UINT>& index
		);

		int getVertexNum();
		int getIndexNum();
		ID3D12Resource* getVertexBuffer();
		ID3D12Resource* getIndexBuffer();
		D3D12_VERTEX_BUFFER_VIEW getVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW getIndexBufferView();
		std::vector<lib::Vertex>& getVertex();
		std::vector<UINT>& getIndex();
	private:
		void mLoadFile(const char* FileName);
		bool mDisplayContent(FbxScene* scene);
		bool mDisplayContent(FbxNode* node);
		void mLoadMesh(FbxNode* pNode);
		bool mLoadIndex(FbxMesh* mesh);
		bool mLoadVertex(FbxMesh* mesh);
		bool mLoadVertexColor(FbxMesh* mesh);
		bool mCreateVertexBuffer(ComPtr<ID3D12Device> device);
		bool mCreateIndexBuffer(ComPtr<ID3D12Device> device);
	};
}