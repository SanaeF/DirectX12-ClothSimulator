#include "VertexPolygonModel.h"

namespace phy {
	VertexPolygonModel::VertexPolygonModel(lib::ModelData& model) {
		int vertex_num = model.vertex.size();
		m_Polygon_model.resize(vertex_num);
		for (int ite = 0; ite < vertex_num; ite++) {
			createPolygonID(ite, model.index, model.index_group);
		}
	}
	void VertexPolygonModel::createPolygonID(
		int id, 
		std::vector<UINT>& index, 
		std::vector<std::vector<int>>& index_group
	) {
		PolygonModel result;
		result.polygon_num = index_group[id].size();
		if (result.polygon_num > 6)result.polygon_num = 6;
		for (int ite = 0; ite < 6; ite++) {
			if (result.polygon_num <= ite)result.id[ite] = DirectX::XMINT3(0, 0, 0);
			else {
				auto index3 = createIndex(id, ite, index_group);
				result.id[ite].x = getVertex(index3[0], index);
				result.id[ite].y = getVertex(index3[1], index);
				result.id[ite].z = getVertex(index3[2], index);
			}
		}
		m_Polygon_model[id] = result;
	}
	std::vector<int> VertexPolygonModel::createIndex(
		int vert_id, 
		int group_id, 
		std::vector<std::vector<int>>& index_group
	) {
		std::vector<int> index(3);
		if (index_group[vert_id][group_id] % 3 == 0) {
			index[0] = index_group[vert_id][group_id];
			index[1] = index_group[vert_id][group_id] + 1;
			index[2] = index_group[vert_id][group_id] + 2;
		}
		if (index_group[vert_id][group_id] % 3 == 1) {
			index[0] = index_group[vert_id][group_id] - 1;
			index[1] = index_group[vert_id][group_id];
			index[2] = index_group[vert_id][group_id] + 1;
		}
		if (index_group[vert_id][group_id] % 3 == 2) {
			index[0] = index_group[vert_id][group_id] - 2;
			index[1] = index_group[vert_id][group_id] - 1;
			index[2] = index_group[vert_id][group_id];
		}
		return index;
	}
	int VertexPolygonModel::getVertex(int id, std::vector<UINT>& index) {
		if (id == -1)return -1;
		return index[id];
	}
	std::vector<PolygonModel> VertexPolygonModel::getData() {
		return m_Polygon_model;
	}
}