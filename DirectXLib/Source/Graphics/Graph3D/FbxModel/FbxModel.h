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
	class ClothSimulatorOld;
}
namespace model {
	class FbxModel {
	public:
		struct ModelData {
			std::vector<lib::Vertex>vertex;
			std::vector<UINT>index;
			std::vector<std::vector<int>>index_group;
			ID3D12Resource* vertex_buffer;
			ID3D12Resource* index_buffer;
			D3D12_VERTEX_BUFFER_VIEW vb_view;
			D3D12_INDEX_BUFFER_VIEW ib_view;
		};
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		ModelData m_Model;
		FbxManager* m_Manager = nullptr;
		FbxScene* m_Scene = nullptr;
		FbxMesh* m_Mesh = nullptr;
		CD3DX12_RANGE m_Read_range;
		std::shared_ptr<lib::ClothSimulatorOld> m_ClothSim;
	public:
		FbxModel();
		~FbxModel();
		void load(const char* FileName);
		void createViewBuffer(ComPtr<ID3D12Device> device, ModelData data);
		ModelData& getModelData();
	private:
		void loadFile(const char* FileName);
		bool displayContent(FbxScene* scene);
		bool displayContent(FbxNode* node);
		void loadMesh(FbxNode* pNode);
		bool loadIndex(FbxMesh* mesh);
		bool loadVertex(FbxMesh* mesh);
		bool loadTexture();
		bool loadIndexNormal(int index_num, FbxArray<FbxVector4> normals);
		bool loadIndecesColor(int index_num, FbxMesh* mesh);
		void loadVitrualPointIndex(const char* FileName);
		bool mCreateVertexBuffer(ComPtr<ID3D12Device> device);
		bool mCreateIndexBuffer(ComPtr<ID3D12Device> device);
	};
}