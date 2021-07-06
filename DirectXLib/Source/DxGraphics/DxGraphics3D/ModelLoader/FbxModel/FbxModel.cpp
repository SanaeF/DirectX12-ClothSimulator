#include "FbxModel.h"
#include "../../../../LibHelper/LibHelper.h"
#include "../../../../DirectX12Manager/DirectX12Manager.h"
namespace model {
	FbxModel::FbxModel()
	{
	}
	void FbxModel::load(const char* FileName) {
		loadFile(FileName);
	}
	void FbxModel::loadFile(const char* FileName) {
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

		displayContent(m_Scene);
		m_Manager->Destroy();
	}
	bool FbxModel::displayContent(FbxScene* scene) {
		FbxNode* node = scene->GetRootNode();
		int nodeCount = node->GetChildCount();
		for (int i = 0; i < nodeCount; i++)
			displayContent(node->GetChild(i));
		return true;
	}
	bool FbxModel::displayContent(FbxNode* node) {
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
				loadMesh(node);
				break;
			}
		}
		for (int i = 0; i < node->GetChildCount(); i++)displayContent(node->GetChild(i));
		return true;
	}

	void FbxModel::loadMesh(FbxNode* node) {
		FbxGeometryConverter converter(m_Manager);
		converter.SplitMeshesPerMaterial(m_Scene, true);//全Mesh分割
		converter.SplitMeshPerMaterial(m_Mesh, true);//Mesh単体分割
		FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
		auto m_Index_num = mesh->GetPolygonVertexCount();//インデックス数
		auto m_Vertex_num = mesh->GetControlPointsCount();//頂点数
		m_Model.index.resize(m_Index_num);
		m_Model.vertex.resize(m_Vertex_num);
		m_Model.index_group.resize(m_Vertex_num);
		if (!loadIndex(mesh))return;
		if (!loadVertex(mesh))return;
		return;
	}
	bool FbxModel::loadVertex(FbxMesh* mesh) {
		if (m_Model.vertex.size() <= 0)return false;
		FbxVector4* vertexData = mesh->GetControlPoints();	// 頂点座標配列
		helper::LibHelper help;
		for (int i = 0; i < m_Model.vertex.size(); i++) {
			m_Model.vertex[i].position.x = (float)vertexData[i][0];
			m_Model.vertex[i].position.y = -(float)vertexData[i][1];
			m_Model.vertex[i].position.z = (float)vertexData[i][2];
		}
		return true;
	}
	bool FbxModel::loadTexture() {
		return true;
	}
	bool FbxModel::loadIndexNormal(int index_num, FbxArray<FbxVector4> normals) {
		int vert_num = m_Model.index[index_num];
		m_Model.vertex[vert_num].normal.x = -normals[index_num][0];
		m_Model.vertex[vert_num].normal.y = normals[index_num][1];
		m_Model.vertex[vert_num].normal.z = normals[index_num][2];
		m_Model.vertex[vert_num].normal.w = normals[index_num][3];
		return true;
	}
	bool FbxModel::loadIndecesColor(int index_num, FbxMesh* mesh) {
		int color_count = mesh->GetElementVertexColorCount();
		if (color_count == 0)return false;
		// 頂点カラーデータの取得
		FbxGeometryElementVertexColor* color_buffer = mesh->GetElementVertexColor(0);
		if (color_buffer == nullptr)return false;
		FbxLayerElement::EMappingMode mapping_mode = color_buffer->GetMappingMode();
		FbxLayerElement::EReferenceMode reference_mode = color_buffer->GetReferenceMode();
		// モードチェック
		if (mapping_mode == FbxLayerElement::eByPolygonVertex) {
			// 参照方法チェック
			if (reference_mode == FbxLayerElement::eIndexToDirect) {
				// 頂点カラーバッファ取得
				FbxLayerElementArrayTemplate<FbxColor>& colors = color_buffer->GetDirectArray();
				// 頂点カラーインデックスバッファ
				FbxLayerElementArrayTemplate<int>& indeces = color_buffer->GetIndexArray();
				int vert_num = m_Model.index[index_num];
				int id = indeces.GetAt(index_num);
				FbxColor color = colors.GetAt(id);
				m_Model.vertex[vert_num].color.x = color[0];
				m_Model.vertex[vert_num].color.y = color[1];
				m_Model.vertex[vert_num].color.z = color[2];
			}
		}
		return true;
	}
	bool FbxModel::loadIndex(FbxMesh* mesh) {
		if (m_Model.index.size() <= 0)return false;
		UINT* index = (UINT*)mesh->GetPolygonVertices();
		FbxArray<FbxVector4> normals;
		mesh->GetPolygonVertexNormals(normals);
		for (int num = 0; num < m_Model.index.size(); num++) {
			m_Model.index[num] = index[num];
			m_Model.index_group[index[num]].push_back(num);
			loadIndexNormal(num, normals);//法線リストの取得
			loadIndecesColor(num, mesh);// 頂点カラーの取得
		}
		return true;
	}
	void FbxModel::loadVitrualPointIndex(const char* FileName) {
		//for (int ite = 0; ite < 8; ite++) {
		//	m_Model.pre_index[ite].resize(m_Model.vertex.size());
		//	m_Model.virtual_vertex[ite].resize(m_Model.vertex.size());
		//}
		//m_Model.virtual_vertex[8].resize(m_Model.vertex.size());
		//for (int ite = 0; ite < m_Model.vertex.size(); ite++) {
		//	VirtualPoint vP(ite, m_Model.vertex, m_Model.index);
		//	auto point = vP.create();
		//	m_Model.virtual_vertex[0][ite] = m_Model.vertex[ite];
		//	for (int ite2 = 0; ite2 < 8; ite2++) {
		//		m_Model.pre_index[ite2][ite] = point[ite2];
		//		if (point[ite2] < m_Model.vertex.size())m_Model.virtual_vertex[1+ite2][ite] = m_Model.vertex[point[ite2]];
		//		else {
		//			/*m_Model.virtual_vertex[1 + ite2][ite].color.x = 1.f;
		//			m_Model.virtual_vertex[1 + ite2][ite].color.y = 0.f;
		//			m_Model.virtual_vertex[1 + ite2][ite].color.z = 0.f;*/
		//		}
		//	}
		//}
	}

	void FbxModel::createViewBuffer(ComPtr<ID3D12Device> device, ModelData data) {
		m_Model = data;
		if (!createVertexBuffer(device))return;
		if (!createIndexBuffer(device))return;
		return;
	}
	bool FbxModel::createVertexBuffer(ComPtr<ID3D12Device> device) {
		const UINT vertexBufferSize = static_cast<UINT>(m_Model.vertex.size()) * sizeof(lib::ModelData);
		constexpr size_t vertexSize = 38;
		ID3D12Resource* vertex_buffer = nullptr;
		auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		auto result = device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&vertex_buffer));//ファイルが無いとエラー
		UINT8* pVertexDataBegin;
		m_Read_range.Begin = 0;
		m_Read_range.End = 0;
		vertex_buffer->Map(
			0, &m_Read_range, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, m_Model.vertex.data(), vertexBufferSize);
		vertex_buffer->Unmap(0, nullptr);
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		vertex_buffer_view.BufferLocation = vertex_buffer->GetGPUVirtualAddress();
		vertex_buffer_view.SizeInBytes = vertexBufferSize;
		vertex_buffer_view.StrideInBytes = sizeof(lib::ModelData);
		m_Model.vertex_buffer = vertex_buffer;
		m_Model.vb_view = vertex_buffer_view;
		return true;
	}
	bool FbxModel::createIndexBuffer(ComPtr<ID3D12Device> device) {
		//helper::LibHelper help;
		const UINT indexBufferSize =
			static_cast<UINT>(m_Model.index.size()) * sizeof(UINT);
		ID3D12Resource* index_buffer = nullptr;
		CD3DX12_HEAP_PROPERTIES heapProperty =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferResource =
			CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		device->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&index_buffer));
		UINT8* pIndexDataBegin;
		index_buffer->Map(
			0, &m_Read_range, reinterpret_cast<void**>(&pIndexDataBegin));
		memcpy(pIndexDataBegin, m_Model.index.data(), indexBufferSize);
		index_buffer->Unmap(0, nullptr);
		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		index_buffer_view.BufferLocation = index_buffer->GetGPUVirtualAddress();
		index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		index_buffer_view.SizeInBytes = indexBufferSize;
		m_Model.index_buffer = index_buffer;
		m_Model.ib_view = index_buffer_view;
		return true;
	}
	FbxModel::ModelData& FbxModel::getModelData() { return m_Model; }
	FbxModel::~FbxModel() {}
}