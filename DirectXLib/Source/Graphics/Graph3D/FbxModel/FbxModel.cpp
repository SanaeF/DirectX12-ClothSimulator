#include "FbxModel.h"
#include "../../../../../DirectXLib/Source/LibHelper/LibHelper.h"
#include "../../DirectX12/DirectXLib/Source/DirectX12/DirectX12Manager.h"
#include "../../../../../DirectXLib/Source/Graphics/Graph3D/ClothSimulator/ClothSimulator.h"

namespace model {
	FbxModel::FbxModel() :
		m_VertexNum(0),
		m_IndexNum(0)
	{
	}
	void FbxModel::load(const char* FileName) {
		mLoadFile(FileName);
	}
	void FbxModel::mLoadFile(const char* FileName) {
		m_Manager = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(m_Manager, IOSROOT);
		m_Manager->SetIOSettings(ios);
		FbxScene* m_Scene = FbxScene::Create(m_Manager, "");
		const char* filename = FileName;
		FbxImporter* importer = FbxImporter::Create(m_Manager, "");
		importer->Initialize(filename, -1, m_Manager->GetIOSettings());
		importer->Import(m_Scene);
		importer->Destroy();
		FbxGeometryConverter geometryConverter(m_Manager);
		geometryConverter.Triangulate(m_Scene, true);

		mDisplayContent(m_Scene);
		m_Manager->Destroy();
	}
	bool FbxModel::mDisplayContent(FbxScene* scene) {
		FbxNode* node = scene->GetRootNode();
		int nodeCount = node->GetChildCount();
		for (int i = 0; i < nodeCount; i++)
			mDisplayContent(node->GetChild(i));
		return true;
	}
	bool FbxModel::mDisplayContent(FbxNode* node) {
		FbxNodeAttribute::EType lAttributeType;

		if (node->GetNodeAttribute() == NULL)return false;
		else {
			lAttributeType = (
				node->GetNodeAttribute()->GetAttributeType()
				);
			switch (lAttributeType) {
			default:
				break;

			case FbxNodeAttribute::eMesh:
				mLoadMesh(node);
				break;
			}
		}

		for (int i = 0; i < node->GetChildCount(); i++)
			mDisplayContent(node->GetChild(i));
		return true;
	}

	void FbxModel::mLoadMesh(FbxNode* node) {
		FbxGeometryConverter converter(m_Manager);
		converter.SplitMeshesPerMaterial(m_Scene, true);//全Mesh分割
		converter.SplitMeshPerMaterial(m_Mesh, true);//Mesh単体分割
		FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
		if (!mLoadIndex(mesh))return;
		if (!mLoadVertex(mesh))return;
		return;
	}
	bool FbxModel::mLoadVertex(FbxMesh* mesh) {
		m_VertexNum = mesh->GetControlPointsCount();	// 頂点数
		if (m_VertexNum <= 0)return false;
		mVertex.resize(m_VertexNum);
		//FbxArr normals;
		FbxArray<FbxVector4> normals;
		// 法線リストの取得
		mesh->GetPolygonVertexNormals(normals);
		FbxVector4* vertexData = mesh->GetControlPoints();	// 頂点座標配列
		for (int i = 0; i < m_VertexNum; i++) {
			mVertex[i].position.x = (float)vertexData[i][2];
			mVertex[i].position.y = (float)vertexData[i][1];
			mVertex[i].position.z = (float)vertexData[i][0];
		}
		mLoadVertexColor(mesh);
		return true;
	}
	bool FbxModel::mLoadVertexColor(FbxMesh* mesh) {
		int color_count = mesh->GetElementVertexColorCount();
		if (color_count == 0)return false;
		// 頂点カラーデータの取得
		FbxGeometryElementVertexColor* color_buffer = mesh->GetElementVertexColor(0);
		if (color_buffer == nullptr)return false;
		FbxLayerElement::EMappingMode mapping_mode = color_buffer->GetMappingMode();
		FbxLayerElement::EReferenceMode reference_mode = color_buffer->GetReferenceMode();
		// モードチェック
		if (mapping_mode == FbxLayerElement::eByPolygonVertex)
		{
			// 参照方法チェック
			if (reference_mode == FbxLayerElement::eIndexToDirect)
			{
				// 頂点カラーバッファ取得
				FbxLayerElementArrayTemplate<FbxColor>& colors = color_buffer->GetDirectArray();
				// 頂点カラーインデックスバッファ
				FbxLayerElementArrayTemplate<int>& indeces = color_buffer->GetIndexArray();

				for (int i = 0; i < m_VertexNum; i++)
				{
					int id = indeces.GetAt(i);
					FbxColor color = colors.GetAt(id);
					mVertex[i].color.x = color.mRed;
					mVertex[i].color.y = 1.f;
					//mVertex[i].color.z = 1.f;
					//mVertex[i].color.w = 1.f;
				}
			}
		}
		return true;
	}
	bool FbxModel::mLoadIndex(FbxMesh* mesh) {
		//総ポリゴン数
		m_IndexNum = mesh->GetPolygonVertexCount();
		if (m_IndexNum <= 0)return false;
		UINT* index = (UINT*)mesh->GetPolygonVertices();
		mIndex.resize(m_IndexNum);
		int Polnum = 0;
		//p個目のポリゴンへの処理
		for (int num = 0; num < m_IndexNum; num++) {
			mIndex[num] = index[num];
		}
		return true;
	}

	void FbxModel::createViewBuffer(ComPtr<ID3D12Device> device) {
		if (!mCreateVertexBuffer(device))return;
		if (!mCreateIndexBuffer(device))return;
		return;
	}
	void FbxModel::setClothSimulator(ComPtr<ID3D12Device> device) {
		mClothSim.reset(new lib::ClothSimulator(device, mVertex, mIndex));
	}
	void FbxModel::calculatePhysics(
		std::shared_ptr<lib::DirectX12Manager> dx12,
		std::vector<lib::Vertex>& vertex,
		std::vector<UINT>& index
	) {
		mClothSim->inputParamater();
		mClothSim->calculate(1, dx12, vertex, index);
	/*	lib::ClothSimulator cloth;
		cloth.inputParamater();
		cloth.calculate(1, vertex, index);*/
		mVertex = vertex;
		createViewBuffer(dx12->Device());
	}

	bool FbxModel::mCreateVertexBuffer(ComPtr<ID3D12Device> device) {
		const UINT vertexBufferSize = static_cast<UINT>(mVertex.size()) * sizeof(lib::Vertex);
		constexpr size_t vertexSize = 38;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		auto result= device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_vertexBuffer));//ファイルが無いとエラー
		UINT8* pVertexDataBegin;
		readRange.Begin = 0;
		readRange.End = 0;
		m_vertexBuffer->Map(
			0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, mVertex.data(), vertexBufferSize);
		m_vertexBuffer->Unmap(0, nullptr);
		m_VertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = vertexBufferSize;
		m_VertexBufferView.StrideInBytes = sizeof(lib::Vertex);
		return true;
	}
	bool FbxModel::mCreateIndexBuffer(ComPtr<ID3D12Device> device) {
		//helper::LibHelper help;
		const UINT indexBufferSize =
			static_cast<UINT>(mIndex.size()) * sizeof(UINT);

		CD3DX12_HEAP_PROPERTIES heapProperty =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferResource =
			CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		device->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_indexBuffer));
		UINT8* pIndexDataBegin;
		m_indexBuffer->Map(
			0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
		memcpy(pIndexDataBegin, mIndex.data(), indexBufferSize);
		m_indexBuffer->Unmap(0, nullptr);
		m_IndexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_IndexBufferView.SizeInBytes = indexBufferSize;
		return true;
	}

	int FbxModel::getVertexNum() { return m_VertexNum; }
	int FbxModel::getIndexNum() { return m_IndexNum; }
	ID3D12Resource* FbxModel::getVertexBuffer() { return m_vertexBuffer; }
	ID3D12Resource* FbxModel::getIndexBuffer() { return m_indexBuffer; }
	D3D12_VERTEX_BUFFER_VIEW FbxModel::getVertexBufferView() { return m_VertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW FbxModel::getIndexBufferView() { return m_IndexBufferView; }
	std::vector<lib::Vertex>&
		FbxModel::getVertex() {	return mVertex;}
	std::vector<UINT>& FbxModel::getIndex() { return mIndex; }
	FbxModel::~FbxModel() {}
}