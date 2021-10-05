#include "MassSpringModel.h"
#include "../../../VectorMath/VectorMath.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
namespace phy {
	MassSpringModel::MassSpringModel(
		MODEL_FILE model_type,
		std::vector<lib::ModelParam>& vertex,
		std::vector<UINT>& index,
		std::vector<std::vector<int>>& index_group
	) :
		m_File_type(model_type),
		m_Vertex(vertex),
		m_Index(index),
		m_Index_group(index_group)
	{
		m_Result.resize(SPRING_NUM);
	}
	std::vector<int> MassSpringModel::create(int num) {
		m_CheckIGP = m_Index_group[num];
		for (int ite = 0; ite < m_Result.size(); ite++)m_Result[ite] = -1;
		if (!isThrowVertex(m_Vertex[num])) {
			for (int ite = 0; ite < 4; ite++) {
				m_Mid_point[ite] = -1;
				m_Cor_point[ite] = -1;
				m_More_point[ite] = -1;
				for (int ite2 = 0; ite2 < 4; ite2++)m_Mid_points[ite][ite2] = -1;
			}
			createMatrix3x3(num);
		}
		return m_Result;
	}
	void MassSpringModel::createMatrix3x3(int vertex_id) {
		if (vertex_id == 2) {
			int a = 0;
		}
		auto all_index = loadAllIndex(vertex_id);
		createMidP(vertex_id, all_index, m_Mid_point);
		for (int ite = 0; ite < 4; ite++) {
			if (m_Mid_point[ite] == -1)continue;
			auto all_index4 = loadAllIndex(m_Index[m_Mid_point[ite]]);
			createMidP(m_Index[m_Mid_point[ite]], all_index4, m_Mid_points[ite]);
		}
		createCorP(vertex_id);
		createMoreP(vertex_id);
		for (int ite = 0; ite < 4; ite++) {
			if (m_Mid_point[ite] != -1)m_Result[ite] = m_Index[m_Mid_point[ite]];
			if (m_Cor_point[ite] != -1)m_Result[4 + ite] = m_Index[m_Cor_point[ite]];
			if (m_More_point[ite] != -1)m_Result[8 + ite] = m_Index[m_More_point[ite]];
		}
	}
	MassSpringModel::IndexData
		MassSpringModel::loadAllIndex(int vertex_id) {
		IndexData data;
		data.reset(m_Index_group[vertex_id].size());
		//仮想頂点用のインデックスを初期化(-1を代入)
		for (int ite = 0; ite < data.constant.size(); ite++) {
			data.constant[ite] = -1;
		}
		//調べている頂点番号と一致するインデックス番号を確保
		auto max = m_Index_group[vertex_id].size();
		for (int num = 0; num < m_Index_group[vertex_id].size(); num++) {
			data.constant[data.count] = m_Index_group[vertex_id][num];
			data.count++;
		}
		return data;
	}
	void MassSpringModel::createMidP(int vertex_id, IndexData& related_index, int* data) {
		if (related_index.count == 0)return;
		//インデックスタイプが
		//2-〇==〇-0     2-〇
		//    ＼||   and   ||＼
		//      〇-1     1-〇==〇-0
		//の時計回りを想定(pmxのみ)
		IndexData result;
		result.reset(related_index.count);
		for (int ite = 0; ite < related_index.count; ite++) {
			if (related_index.constant[ite] % 3 == 0) {
				result.constant[result.count] = related_index.constant[ite] + 1;
				result.count++;
			}
			if (related_index.constant[ite] % 3 == 1) {
				result.constant[result.count] = related_index.constant[ite] - 1;
				result.count++;
			}
		}
		result = deleteOverIndex(result);
		//ソート
		result = sortIndex(vertex_id, result.count, result);
		auto test = checkIndexID(result);
		//代入
		for (int ite = 0; ite < result.count; ite++) {
			if (ite < 4)data[ite] = result.constant[ite];
		}
	}
	//void MassSpringModel::createDist(int vertex_id, float& high, int* data) {
	//	// 中心から一番遠い距離を取得
	//	float size_max = 0.f;
	//	for (int ite = 0; ite < 4; ite++) {
	//		if (data[ite] == -1)continue;
	//		auto id = m_Index[data[ite]];
	//		auto dist = lib::VectorMath::distance(
	//			m_Vertex[vertex_id].position, m_Vertex[id].position
	//		);
	//		if (size_max < dist)size_max = dist;
	//	}
	//	high = size_max;
	//	size_max = 0.f;
	//	for (int ite = 0; ite < 4; ite++) {
	//		if (data[ite] == -1)continue;
	//		auto id = m_Index[data[ite]];
	//		auto dist = lib::VectorMath::distance(
	//			m_Vertex[vertex_id].position, m_Vertex[id].position
	//		);
	//		if (dist < high && size_max < dist)size_max = dist;
	//	}
	//	high = size_max;
	//}
	//void MassSpringModel::createDistUsingTextureMap(int vertex_id, float& high, int* data) {
	//	// 中心から一番遠い距離を取得
	//	float size_max = 0.f;
	//	for (int ite = 0; ite < 4; ite++) {
	//		if (data[ite] == -1)continue;
	//		auto id = m_Index[data[ite]];
	//		auto dist = lib::VectorMath::distanceF2(
	//			m_Vertex[vertex_id].tex, m_Vertex[id].tex
	//		);
	//		if (size_max < dist)size_max = dist;
	//	}
	//	high = size_max;
	//	size_max = 0.f;
	//	for (int ite = 0; ite < 4; ite++) {
	//		if (data[ite] == -1)continue;
	//		auto id = m_Index[data[ite]];
	//		auto dist = lib::VectorMath::distanceF2(
	//			m_Vertex[vertex_id].tex, m_Vertex[id].tex
	//		);
	//		if (dist < high && size_max < dist)size_max = dist;
	//	}
	//	high = size_max;
	//}
	void MassSpringModel::createCorP(int vertex_id) {
		IndexData sort_data;
		sort_data.reset(16);
		for (int ite = 0; ite < 2; ite++) {
			for (int ite2 = 0; ite2 < 2; ite2++) {
				auto& idx_id = m_Mid_points[ite][2+ite2];//2～2は長い方からにある2質点
				if (idx_id == -1)continue;
				bool input = true;
				//十字方向の質点は除外
				for (int ite3 = 0; ite3 < 4; ite3++) {
					if (m_Mid_point[ite3] != -1) {
						if (m_Index[idx_id] == m_Index[m_Mid_point[ite3]])input = false;
					}
				}
				//基準質点は除外
				if (vertex_id == m_Index[idx_id])input = false;
				if (input) {
					sort_data.constant[sort_data.count] = idx_id;
					sort_data.count++;
				}
			}
		}
		sort_data = deleteNullIndex(sort_data);
		auto test = checkIndexID(sort_data);
		for (int ite = 0; ite < sort_data.count; ite++) {
			if (ite < 4)m_Cor_point[ite] = sort_data.constant[ite];
		}
	}
	void MassSpringModel::createMoreP(int vertex_id) {
		IndexData sort_data;
		sort_data.reset(16);
		IndexData created_data;
		created_data.reset(9);
		for (int ite = 0; ite < 4; ite++) {
			//3x3のデータをまとめる
			created_data.constant[created_data.count] = m_Mid_point[ite];
			created_data.count++;
			created_data.constant[created_data.count] = m_Cor_point[ite];
			created_data.count++;
			//全質点をデータにまとめる
			for (int ite2 = 0; ite2 < 4; ite2++) {
				sort_data.constant[sort_data.count] = m_Mid_points[ite][ite2];
				sort_data.count++;
			}
		}
		for (int ite = 0; ite < sort_data.count; ite++) {
			if (sort_data.constant[ite] == -1)continue;
			bool is_delete = false;
			//3x3の質点を除外する
			for (int ite2 = 0; ite2 < created_data.count; ite2++) {
				if (created_data.constant[ite2] == -1)continue;
				if (m_Index[sort_data.constant[ite]] == m_Index[created_data.constant[ite2]])is_delete = true;
			}
			//基準となる質点を除外する
			if (vertex_id == m_Index[sort_data.constant[ite]])is_delete = true;
			if (is_delete)sort_data.constant[ite] = -1;
		}
		sort_data = deleteNullIndex(sort_data);
		for (int ite = 0; ite < sort_data.count; ite++) {
			if (ite < 4)m_More_point[ite] = sort_data.constant[ite];
		}
	}
	MassSpringModel::IndexData 
		MassSpringModel::sortIndex(int vertex_id, int sort_max, IndexData& data) {
		data = deleteNullIndex(data);
		//距離が遠い順に並べる
		std::vector<int> sort_data(data.count);
		for (int ite = 0; ite < data.count; ite++) {
			int max_id = 0;
			float max_dist = 0.f;
			for (int ite2 = 0; ite2 < data.count; ite2++) {
				//格納データの中から、一致するインデックスIDがあるかどうか
				bool is_continue = false;
				for (int ite3 = 0; ite3 < sort_data.size(); ite3++) {
					if (sort_data[ite3] == data.constant[ite2])is_continue = true;
				}
				if (is_continue)continue;
				auto id = m_Index[data.constant[ite2]];
				auto dist = lib::VectorMath::distance(
					m_Vertex[vertex_id].position, m_Vertex[id].position
				);
				//一番距離が遠ければ最大値(IDと距離)を更新する
				if (max_dist < dist) {
					max_dist = dist;
					max_id = data.constant[ite2];
				}
			}
			//最大距離のインデックスIDを格納する
			sort_data[ite] = max_id;
		}
		//逆順にする
		IndexData result;
		result.reset(sort_data.size());
		for (int ite = 0; ite < sort_data.size(); ite++) {
			result.constant[sort_data.size() - result.count - 1] = sort_data[ite];
			result.count++;
		}
		result.constant.resize(sort_max);
		result.count = sort_max;
		return result;
	}
	MassSpringModel::IndexData 
	MassSpringModel::sortIndexUsingTextureMap(int vertex_id, int sort_max, IndexData& data) {
		data = deleteNullIndex(data);
		//距離が遠い順に並べる
		std::vector<int> sort_data(data.count);
		for (int ite = 0; ite < data.count; ite++) {
			int max_id = 0;
			float max_dist = 0.f;
			for (int ite2 = 0; ite2 < data.count; ite2++) {
				//格納データの中から、一致するインデックスIDがあるかどうか
				bool is_continue = false;
				for (int ite3 = 0; ite3 < sort_data.size(); ite3++) {
					if (sort_data[ite3] == data.constant[ite2])is_continue = true;
				}
				if (is_continue)continue;
				auto id = m_Index[data.constant[ite2]];
				auto dist = lib::VectorMath::distanceF2(
					m_Vertex[vertex_id].tex, m_Vertex[id].tex
				);
				//一番距離が遠ければ最大値(IDと距離)を更新する
				if (max_dist < dist) {
					max_dist = dist;
					max_id = data.constant[ite2];
				}
			}
			//最大距離のインデックスIDを格納する
			sort_data[ite] = max_id;
		}
		//逆順にする
		IndexData result;
		result.reset(sort_data.size());
		for (int ite = 0; ite < sort_data.size(); ite++) {
			result.constant[sort_data.size() - result.count - 1] = sort_data[ite];
			result.count++;
		}
		result.constant.resize(sort_max);
		result.count = sort_max;
		return result;
	}
	MassSpringModel::IndexData 
	MassSpringModel::deleteNullIndex(IndexData& data) {
		IndexData result;
		result.reset(data.count);
		for (int ite = 0; ite < data.count; ite++) {
			if (data.constant[ite] != -1) {
				result.constant[result.count] = data.constant[ite];
				result.count++;
			}
		}
		return result;
	}
	MassSpringModel::IndexData 
	MassSpringModel::deleteOverIndex(IndexData& data) {
		helper::LibHelper help;
		for (int ite = 0; ite < data.count; ite++) {
			if (!help.between(data.constant[ite], 0, m_Index.size()))data.constant[ite] = -1;
		}
		for (int ite = 0; ite < data.count; ite++) {
			if (data.constant[ite] == -1)continue;
			bool is_delete = false;
			for (int ite2 = 0; ite2 < data.count; ite2++) {
				if (data.constant[ite] == data.constant[ite2] || data.constant[ite2] == -1)continue;
				if (m_Index[data.constant[ite]] == m_Index[data.constant[ite2]])is_delete = true;
			}
			if(is_delete)data.constant[ite] = -1;
		}
		return deleteNullIndex(data);
	}
	MassSpringModel::IndexData 
	MassSpringModel::changeMatrixData(int num, int* data) {
		IndexData result;
		result.reset(num);
		for (int ite = 0; ite < num; ite++) {
			result.constant[result.count] = data[ite];
			result.count++;
		}
		return result;
	}
	std::vector<int> 
		MassSpringModel::checkIndexID(IndexData& data) {
		IndexData result;
		result.reset(data.count);
		for (int ite = 0; ite < data.count; ite++) {
			if (data.constant[ite] != -1) {
				result.constant[result.count] = m_Index[data.constant[ite]];
				result.count++;
			}
		}
		return result.constant;
	}
	bool MassSpringModel::isThrowVertex(lib::ModelParam& vert) {
		if (vert.color.x == 1.0f && 
			vert.color.y == 0.0f && 
			vert.color.z == 0.0f
			)return true;
		return false;
	}
	MassSpringModel::~MassSpringModel() {

	}
}