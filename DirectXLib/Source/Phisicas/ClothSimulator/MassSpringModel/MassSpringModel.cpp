#include "MassSpringModel.h"
#include "../../../VectorMath/VectorMath.h"
#include "../../DirectXLib/Source/LibHelper/LibHelper.h"
namespace phy {
	MassSpringModel::MassSpringModel(
		MODEL_FILE model_type,
		std::vector<lib::ModelVertex>& vertex,
		std::vector<UINT>& index,
		std::vector<std::vector<int>>& index_group
	) :
		m_File_type(model_type),
		m_Vertex(vertex),
		m_Index(index),
		m_Index_group(index_group)
	{
		m_Result.resize(SPRING_NUM);
		m_Mass_model.resize(m_Vertex.size());
		for (int ite = 0; ite < m_Vertex.size(); ite++) {
			auto m3x3_4 = create(ite);
			m_Mass_model[ite].id0 = m3x3_4[0];
			m_Mass_model[ite].id1 = m3x3_4[1];
			m_Mass_model[ite].id2 = m3x3_4[2];
			m_Mass_model[ite].id3 = m3x3_4[3];
			m_Mass_model[ite].id4 = m3x3_4[4];
			m_Mass_model[ite].id5 = m3x3_4[5];
			m_Mass_model[ite].id6 = m3x3_4[6];
			m_Mass_model[ite].id7 = m3x3_4[7];
			m_Mass_model[ite].id8 = m3x3_4[8];
			m_Mass_model[ite].id9 = m3x3_4[9];
			m_Mass_model[ite].id10 = m3x3_4[10];
			m_Mass_model[ite].id11 = m3x3_4[11];
		}
	}
	std::vector<MassModel> MassSpringModel::getData() {
		return m_Mass_model;
	}
	std::vector<int> MassSpringModel::create(int num) {
		m_Is_bad_num = false;
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
		if (vertex_id == 498 || vertex_id == 9120) {
			int a = 0;
		}
		createCross(vertex_id, m_Mid_point);
		for (int ite = 0; ite < 4; ite++) {
			if (m_Mid_point[ite] == -1)continue;
			createCross(m_Index[m_Mid_point[ite]], m_Mid_points[ite]);
		}
		createCorner(vertex_id, m_Mid_points, m_Cor_point);
		createMoreP(vertex_id);
		for (int ite = 0; ite < 4; ite++) {
			if (m_Mid_point[ite] != -1)m_Result[ite] = m_Index[m_Mid_point[ite]];
			if (m_Cor_point[ite] != -1)m_Result[4 + ite] = m_Index[m_Cor_point[ite]];
			if (m_More_point[ite] != -1)m_Result[8 + ite] = m_Index[m_More_point[ite]];
		}
	}
	void MassSpringModel::createCross(int vertex_id, int* data) {
		auto all_index = loadAllIndex(vertex_id);
		IndexData result;
		result.reset(18);
		//頂点から得られる周囲の頂点
		for (int ite = 0; ite < all_index.count; ite++) {
			if (all_index.constant[ite] % 3 == 0) {
				result.constant[result.count] = all_index.constant[ite] + 1;
				result.count++;
				result.constant[result.count] = all_index.constant[ite] + 2;
				result.count++;
			}
			if (all_index.constant[ite] % 3 == 1) {
				result.constant[result.count] = all_index.constant[ite] - 1;
				result.count++;
				result.constant[result.count] = all_index.constant[ite] + 1;
				result.count++;
			}
			if (all_index.constant[ite] % 3 == 2) {
				result.constant[result.count] = all_index.constant[ite] - 1;
				result.count++;
				result.constant[result.count] = all_index.constant[ite] - 2;
				result.count++;
			}
		}
		result = deleteOverIndex(result);
		//indexの距離が近い順にソート
		result = sortIndex(vertex_id, result.count, result);
		//dataに代入
		int input_num = 0;
		if (all_index.count == 6)input_num = 4;//中央
		if (all_index.count == 3)input_num = 3;//辺
		if (all_index.count < 3)input_num = 2;//角
		for (int ite = 0; ite < 4; ite++) {
			if (ite >= input_num)data[ite] = -1;
			else data[ite] = result.constant[ite];
		}
	}
	void MassSpringModel::createCorner(int vertex_id, int mid_points[4][4], int* data) {
		//4つの十字質点をまとめる(重複ありのまま)
		IndexData p4x4;
		p4x4.reset(4 * 4);
		for (int ite = 0; ite < 4; ite++) {
			for (int ite2 = 0; ite2 < 4; ite2++) {
				p4x4.constant[p4x4.count] = mid_points[ite][ite2];
				p4x4.count++;
			}
		}
		//重複しているindexID質点のみを結果に代入
		IndexData result;
		result.reset(4 * 4);
		for (int ite = 0; ite < p4x4.count; ite++) {
			if (p4x4.constant[ite] == -1)continue;//不正なデータは除外
			if (m_Index[p4x4.constant[ite]] == vertex_id)continue;//基準質点は除外
			for (int ite2 = 0; ite2 < p4x4.count; ite2++) {
				if (p4x4.constant[ite2] == -1)continue;//不正なデータは除外
				if (m_Index[p4x4.constant[ite2]] == vertex_id)continue;//基準質点は除外
				if (ite == ite2)continue;//同じ配列番号はカウントしない
				if (m_Index[p4x4.constant[ite]] == m_Index[p4x4.constant[ite2]]) {
					result.constant[result.count] = p4x4.constant[ite];
					result.count++;
				}
			}
		}
		result = deleteOverIndex(result);
		for (int ite = 0; ite < 4; ite++) {
			if (ite >= result.count)data[ite] = -1;
			else data[ite] = result.constant[ite];
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
				if (ite == ite2 || data.constant[ite2] == -1)continue;
				if (m_Index[data.constant[ite]] == m_Index[data.constant[ite2]])is_delete = true;
			}
			if (is_delete)data.constant[ite] = -1;
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
	bool MassSpringModel::isThrowVertex(lib::ModelVertex& vert) {
		if (vert.color.x == 1.0f &&
			vert.color.y == 0.0f &&
			vert.color.z == 0.0f
			)return true;
		return false;
	}
	MassSpringModel::~MassSpringModel() {

	}
}