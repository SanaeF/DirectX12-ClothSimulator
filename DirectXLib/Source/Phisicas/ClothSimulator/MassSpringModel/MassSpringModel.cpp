#include "MassSpringModel.h"
#include "../../../VectorMath/VectorMath.h"

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
		m_Result.resize(8);
	}
	std::vector<int> MassSpringModel::create(int num) {
		for (int ite = 0; ite < 8; ite++)m_Result[ite] = -1;
		createMatrix3x3(num);
		return m_Result;
	}
	//3x3行列の質点モデルを生成
	void MassSpringModel::createMatrix3x3(int vertex_id) {
		//対応する全インデックスを取得
		auto all_index = loadAllIndex(vertex_id);
		//自身以外の周囲(3x3)のインデックスを取得
		auto related_index = loadRelatedIndex(vertex_id, all_index);
		if (m_File_type == MODEL_FILE::PMX)related_index = loadMoreRelatedIndex(vertex_id, related_index);
		//周囲(3x3)のインデックス数に応じてタイプを分ける
		m_Edge_type = pointType(all_index, related_index);
		//角の場合(推定 vertex_id + 3 個の頂点)
		if (m_Edge_type == EDGE_TYPE::CORNER) {
			for (int ite = 0; ite < 2; ite++)m_Result[ite] = m_Index[related_index.constant[ite]];
			m_Result[4] = m_Index[related_index.constant[3]];
		}
		//端の場合(推定 vertex_id + 5 個の頂点)
		if (m_Edge_type == EDGE_TYPE::EDGE) {
			for (int ite = 0; ite < 3; ite++)m_Result[ite] = m_Index[related_index.constant[ite]];
			for (int ite = 0; ite < 2; ite++)m_Result[4 + ite] = m_Index[related_index.constant[3 + ite]];
		}
		//端でない場合(推定 vertex_id + 8 個の頂点)
		if (m_Edge_type == EDGE_TYPE::NONE) {
			for (int ite = 0; ite < related_index.constant.size(); ite++) {
				m_Result[ite] = m_Index[related_index.constant[ite]];
			}
		}
	}
	//端かどうかを調べる
	MassSpringModel::EDGE_TYPE
		MassSpringModel::pointType(IndexData& all_index, IndexData& related_index) {
		if (related_index.count == 3)return EDGE_TYPE::CORNER;//角でした。
		if (related_index.count == 5)return EDGE_TYPE::EDGE;//辺だよ。
		if (related_index.count == 8)return EDGE_TYPE::NONE;//結局、端じゃない
		//assert(0 && "布シミュレーターに対応出来ないモデルを使用しています。ごめんね");
		return EDGE_TYPE::INDEX_ERROR;
	}
	//調べている頂点番号と一致するインデックス番号を全て取得
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
	//関連のある頂点番号を全て取得
	MassSpringModel::IndexData
		MassSpringModel::loadRelatedIndex(int vertex_id, IndexData& all_index) {
		IndexData data;
		data.reset(48);
		if (m_File_type == MODEL_FILE::FBX) {
			//取得したインデックスに対応するポリゴンの全てのインデックスを取得
			for (int ite = 0; ite < all_index.count; ite++) {
				if (all_index.constant[ite] != -1) {
					int id = all_index.constant[ite] % 6;
					for (int ite2 = 0; ite2 < 6; ite2++) {
						auto id2 = all_index.constant[ite] - id + ite2;
						//同一のインデックスを除外
						bool is_input = true;
						for (int ite3 = 0; ite3 < data.count; ite3++) {
							if (m_Index[id2] == m_Index[data.constant[ite3]]) {
								is_input = false;
								break;
							}
						}
						if (is_input)for (int ite3 = 0; ite3 < all_index.count; ite3++) {
							if (m_Index[id2] == m_Index[all_index.constant[ite3]]) {
								is_input = false;
								break;
							}
						}
						if (is_input) {
							if (m_Index[id2] == vertex_id)is_input = false;
						}
						if (!is_input)continue;
						data.constant[data.count] = id2;
						data.count++;
					}
				}
			}
			return data;
		}
		if (m_File_type == MODEL_FILE::PMX) {
			//頂点が同一のインデックスに対応する三角形のインデックス番号をすべて取得する
			for (int ite = 0; ite < all_index.count; ite++) {
				for (int ite2 = 0; ite2 < 3; ite2++) {
					auto new_id = all_index.constant[ite] - (all_index.constant[ite] % 3) + ite2;
					//取得済みデータの中から一致しているデータがあるかを取得し、除外する
					bool is_input = true;
					if (new_id < 0)continue;
					if (vertex_id == m_Index[new_id])is_input = false;
					for (int ite3 = 0; ite3 < data.constant.size(); ite3++) {
						if (data.constant[ite3] < 0)break;
						if (m_Index[data.constant[ite3]] == m_Index[new_id]) {
							is_input = false;
							break;
						}
					}
					if (is_input) {
						data.constant[data.count] = new_id;
						data.count++;
					}
				}
			}
		}
		return data;
	}
	//取得したインデックスからさらに対応する三角形のインデックスを取得する
	MassSpringModel::IndexData 
	MassSpringModel::loadMoreRelatedIndex(int vertex_id, IndexData& related_index) {
		IndexData data;
		data.reset(m_Vertex.size());
		//引数のインデックスリストから、対応する三角形のインデックスを取得する
		for (int ite = 0; ite < related_index.count; ite++) {
			auto id = m_Index[related_index.constant[ite]];
			auto all_index2 = loadAllIndex(id);
			auto related_index2 = loadRelatedIndex(id, all_index2);
			for (int ite2 = 0; ite2 < related_index2.count; ite2++) {
				data.constant[data.count] = related_index2.constant[ite2];
				data.count++;
			}
		}
		//引数のインデックスをリストに加える
		for (int ite = 0; ite < related_index.count; ite++) {
			data.constant[data.count] = related_index.constant[ite];
			data.count++;
		}
		//同一インデックスを除外
		for (int ite = 0; ite < data.count; ite++) {
			for (int ite2 = 0; ite2 < data.count; ite2++) {
				if (ite == ite2)continue;
				if (data.constant[ite] < 0 || data.constant[ite2] < 0)continue;
				auto id1 = m_Index[data.constant[ite]];
				auto id2 = m_Index[data.constant[ite2]];
				if (id1 == id2 || id1 == vertex_id) {
					data.constant[ite] = -1;
					break;
				}
			}
		}
		int num = 0;
		if (related_index.count == 2)num = 3;
		if (related_index.count == 4)num = 5;
		if (related_index.count == 6)num = 8;
		auto result = sortIndex(vertex_id, data);
		return result;
	}
	//十字方向の質点を取得
	std::vector<int>
		MassSpringModel::loadNearestIndex(int num, int target_index, IndexData& related_index) {
		IndexData result;
		result.reset(num);
		auto sort_data = sortIndex(target_index, related_index);
		for (int ite = 0; ite < num; ite++) {
			result.constant[ite] = sort_data.constant[ite];
		}
		return result.constant;
	}
	//調べた十字方向の質点以外を取得
	std::vector<int>
		MassSpringModel::loadCorners(int num, IndexData& related_index, std::vector<int>& four_point) {
		IndexData data;
		data.reset(36);
		//取得した周囲のインデックスデータを調べる
		for (int ite = 0; ite < related_index.count; ite++) {
			bool is_input = true;
			//十字方向の質点を対象から除外する
			for (int ite2 = 0; ite2 < four_point.size(); ite2++) {
				if (related_index.constant[ite] == four_point[ite2]) {
					is_input = false;
					break;
				}
			}
			if (is_input) {
				data.constant[data.count] = related_index.constant[ite];
				data.count++;
			}
		}
		data.constant.resize(num);
		return data.constant;
	}
	MassSpringModel::IndexData 
		MassSpringModel::exclusionMatrix3x3(int vertex_id, std::vector<int>& out_of_matrix_index) {
		auto data = out_of_matrix_index;
		for (int ite = 0; ite < data.size(); ite++) {
			for (int ite2 = 0; ite2 < m_Result.size(); ite2++) {
				if (data[ite] == -1)continue;
				auto id = m_Index[data[ite]];
				if (id == m_Result[ite2] || id == vertex_id) {
					if (m_Result[ite2] != -1)data[ite] = -1;
				}
			}
		}
		IndexData result;
		result.reset(data.size());
		for (int ite = 0; ite < data.size(); ite++) {
			if (data[ite] != -1) {
				result.constant[result.count] = data[ite];
				result.count++;
			}
		}
		result.constant.resize(result.count);
		return result;
	}
	MassSpringModel::IndexData 
		MassSpringModel::sortIndex(int vertex_id, IndexData& data) {
		//不明データを除外したデータの生成
		IndexData new_data;
		new_data.reset(data.constant.size());
		for (int ite = 0; ite < data.constant.size(); ite++) {
			if (data.constant[ite] < 0)continue;
			new_data.constant[new_data.count] = data.constant[ite];
			new_data.count++;
		}
		new_data.constant.resize(new_data.count);

		//距離が遠い順に並べる
		std::vector<int> sort_data(new_data.count);
		for (int ite = 0; ite < new_data.count; ite++) {
			int max_id = 0;
			float max_dist = 0.f;
			for (int ite2 = 0; ite2 < new_data.count; ite2++) {
				//格納データの中から、一致するインデックスIDがあるかどうか
				bool is_continue = false;
				for (int ite3 = 0; ite3 < sort_data.size(); ite3++) {
					if (sort_data[ite3] == new_data.constant[ite2])is_continue = true;
				}
				if (is_continue)continue;
				auto id = m_Index[new_data.constant[ite2]];
				auto dist = lib::VectorMath::distance(
					m_Vertex[vertex_id].position, m_Vertex[id].position
				);
				//一番距離が遠ければ最大値(IDと距離)を更新する
				if (max_dist < dist) {
					max_dist = dist;
					max_id = new_data.constant[ite2];
				}
			}
			//最大距離のインデックスIDを格納する
			sort_data[ite] = max_id;
		}

		IndexData result;
		result.reset(sort_data.size());
		for (int ite = 0; ite < sort_data.size(); ite++) {
			result.constant[sort_data.size() - result.count - 1] = sort_data[ite];
			result.count++;
		}
		result.constant.resize(8);
		result.count = 8;
		return result;
	}
	MassSpringModel::~MassSpringModel() {

	}
}